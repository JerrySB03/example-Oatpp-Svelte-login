#include "Users.hpp"

oatpp::Object<AuthDTO> Users::CreateUser(const oatpp::Object<UserDTO> &dto) {
    OATPP_ASSERT_HTTP(dto->email != nullptr, Status::CODE_400, "Email must be set");
    OATPP_ASSERT_HTTP(dto->username != nullptr, Status::CODE_400, "Username must be set");

    if (ExistsWithUsername(dto->username)) {
        throw oatpp::web::protocol::http::HttpError(Status::CODE_409, (oatpp::String) "Username " + dto->username + (oatpp::String) " is already taken");
    } else if (ExistsWithEmail(dto->email)) {
        throw oatpp::web::protocol::http::HttpError(Status::CODE_409, "Email is already taken");
    }

    OATPP_ASSERT_HTTP(!isValidEmail(dto->username), Status::CODE_400, "Username cannot be an email address");
    OATPP_ASSERT_HTTP(isValidEmail(dto->email), Status::CODE_400, "Invalid email address " + *dto->email);

    OATPP_ASSERT_HTTP(dto->password != nullptr, Status::CODE_400, "Password must be set");
    OATPP_ASSERT_HTTP(dto->password->length() >= 8, Status::CODE_400, "Password must be at least 8 characters long");

    auto hash = generateHash(dto->password);
    auto model = UserModel::createShared();
    model->username = dto->username;
    model->email = dto->email;
    model->hash = hash;

    auto dbResult = m_userDb->createUser(model);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, "DATABASE-" + dbResult->getErrorMessage());

    auto payload = std::make_shared<JWT::Payload>();
    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<UserModel>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");
    payload->userId = result[0]->id;

    auto auth = AuthDTO::createShared();
    auth->token = jwt->createToken(payload);

    OATPP_ASSERT_HTTP(auth->token != nullptr, Status::CODE_500, "Error creating token");  // Verify that the token was succesfuly created

    return auth;
}
oatpp::Object<UserDTO> Users::updateUser(const oatpp::Object<UserDTO> &dto) {
    auto model = UserModel::createShared();
    model->id = dto->id;
    model->username = dto->username;
    model->email = dto->email;

    auto dbResult = m_userDb->updateUser(model);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, "DATABASE-" + dbResult->getErrorMessage());

    return getUserById(model->id, dbResult->getConnection());
}
oatpp::postgresql::Uuid Users::getUserId(const oatpp::Object<UserDTO> &dto) {
    auto model = UserModel::createShared();
    model->username = dto->username;
    model->email = dto->email;

    auto dbResult = m_userDb->getUserId(model);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_400, "Invalid username or email");
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "User with provided credentials doesn't exist");

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<UserModel>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Invalid username or email");

    return result[0]->id;
}
oatpp::String Users::getHashData(const Uuid &id) {
    auto dbResult = m_userDb->getHashData(id);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, "DATABASE-" + dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_401, "Invalid credentials!");

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<UserModel>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");
    OATPP_ASSERT_HTTP(result[0]->hash != nullptr, Status::CODE_500, "Hash returned from database was null");
    return result[0]->hash;
}

oatpp::Object<UserDTO> Users::getUserById(const Uuid &id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection> &connection) {
    auto dbResult = m_userDb->getUserById(id, connection);

    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, "DATABASE-" + dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404,
                      "User with id " + id->toString() + " not found");

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<UserModel>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

    auto dto = UserDTO::createShared();
    dto->id = result[0]->id->toString();
    dto->username = result[0]->username;
    dto->email = result[0]->email;
    return dto;
}
bool Users::ExistsWithId(const Uuid &id){
    auto dbResult = m_userDb->getUserById(id);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, "DATABASE-" + dbResult->getErrorMessage());
    return dbResult->hasMoreToFetch();  // If there are more than 0 rows, the user exists
}
oatpp::Object<AuthDTO> Users::validateLogin(const oatpp::Object<UserDTO> &dto) {
    Uuid id;
    try {
        id = getUserId(dto);  // Get the user id from the database
    } catch (oatpp::web::protocol::http::HttpError &e) {
        throw oatpp::web::protocol::http::HttpError(Status::CODE_401, "Invalid credentials!");
    }
    dto->id = id->toString();  // Set the id in the dto
    auto hash = getHashData(id);
    int rc = verifyPassword(hash, dto->password);  // Verify the password
    if (rc != ARGON2_OK) {
        if (rc == ARGON2_VERIFY_MISMATCH) {  // If the password is incorrect
            throw oatpp::web::protocol::http::HttpError(Status::CODE_401, "Invalid credentials!");
        } else {  // If there was an error verifying the password
            throw oatpp::web::protocol::http::HttpError(Status::CODE_500, argon2_error_message(rc));
        }
    }

    auto payload = std::make_shared<JWT::Payload>();
    payload->userId = dto->id;

    auto auth = AuthDTO::createShared();

    auth->token = jwt->createToken(payload);

    OATPP_ASSERT_HTTP(auth->token != nullptr, Status::CODE_500, "Error creating token");  // Verify that the token was succesfuly created
    return auth;
}
oatpp::String Users::generateHash(const oatpp::String &password) {
    uint8_t salt[SALTLEN];
    oatpp::utils::random::Random::randomBytes(salt, SALTLEN);                           // Creating random bytes for the salt
    size_t encHashLen = argon2_encodedlen(1, 1 << 12, 1, SALTLEN, HASHLEN, Argon2_id);  // Get the length of the encoded hash
    char *hash = new char[encHashLen];                                                  // Allocate memory for the encoded hash

    int rc = argon2id_hash_encoded(
        1,
        1 << 12,
        1,
        password->c_str(),
        password->length(),
        salt,
        SALTLEN,
        HASHLEN,
        hash,
        encHashLen);

    OATPP_ASSERT_HTTP(rc == ARGON2_OK, Status::CODE_500, argon2_error_message(rc));  // Verify that the hash was succesfuly created

    oatpp::String hashStr = oatpp::String(hash, encHashLen - 1);  // Minus one, so that it would not include the null byte terminator
    delete[] hash;                                                // Delete the char array to prevent memory leaks

    return hashStr;
}
int Users::verifyPassword(const oatpp::String &hash, const oatpp::String &password) {
    int rc = argon2id_verify(hash->c_str(), password->c_str(), password->length());  // Verify the password
    return (rc);
}
inline bool Users::isValidEmail(const oatpp::String &email) {
    static const std::regex EMAIL_REGEX("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,8}$");
    return std::regex_match(email->c_str(), EMAIL_REGEX);
}
bool Users::ExistsWithUsername(const oatpp::String &username) {
    auto dbResult = m_userDb->getUserByUsername(username);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, "DATABASE-" + dbResult->getErrorMessage());
    return dbResult->hasMoreToFetch();  // If there are more than 0 rows, the user exists
}
bool Users::ExistsWithEmail(const oatpp::String &email) {
    auto dbResult = m_userDb->getUserByEmail(email);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, "DATABASE-" + dbResult->getErrorMessage());
    return dbResult->hasMoreToFetch();  // If there are more than 0 rows, the user exists
}