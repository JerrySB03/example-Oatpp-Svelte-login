#ifndef Users_hpp
#define Users_hpp

#include <argon2.h>

#include <iostream>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/Countable.hpp>
#include <oatpp/core/data/mapping/type/Vector.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/core/utils/Random.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include <regex>

#include "Auth/Auth.hpp"
#include "DTOs/UserDTOs.hpp"
#include "Database/DatabaseComponent.hpp"

/*
 * The Users class is responsible for translating DTOs(Data Transfer Objects) to database models and vice versa.
 * It also checks if the data is valid.
 * It also contains the functions for hashing and verifying passwords and logging in and using JWT.
 */
class Users {

   private:
    OATPP_COMPONENT(std::shared_ptr<UserDb>, m_userDb);  // Get user database component
    OATPP_COMPONENT(std::shared_ptr<JWT>, jwt);          // Get jwt component

    using Status = oatpp::web::protocol::http::Status;
    using Uuid = oatpp::postgresql::Uuid;

    int verifyPassword(const oatpp::String &data, const oatpp::String &password);
    oatpp::String generateHash(const oatpp::String &password);
    static inline bool isValidEmail(const oatpp::String &email);
    bool ExistsWithUsername(const oatpp::String &username);
    bool ExistsWithEmail(const oatpp::String &email);

   public:
    oatpp::Object<AuthDTO> CreateUser(const oatpp::Object<UserDTO> &dto);
    oatpp::Object<UserDTO> updateUser(const oatpp::Object<UserDTO> &dto);
    oatpp::String getHashData(const Uuid &id);
    oatpp::postgresql::Uuid getUserId(const oatpp::Object<UserDTO> &dto);
    oatpp::Object<UserDTO> getUserById(const Uuid &id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection> &connection = nullptr);
    bool ExistsWithId(const Uuid &id);
    oatpp::Object<AuthDTO> validateLogin(const oatpp::Object<UserDTO> &dto);

};

#endif /* Users_hpp */