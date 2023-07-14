#ifndef UserController_hpp
#define UserController_hpp

#include <oatpp-postgresql/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "DTOs/StatusDTO.hpp"
#include "DTOs/UserDTOs.hpp"
#include "Users/Users.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)  //<- Begin Codegen

/**
 * User REST API controller.
 */
class UserController : public oatpp::web::server::api::ApiController {
    /*
     * ENDPOINT_INFO macro adds info about the endpoint to SwaggerUI documentation
     * ENDPOINT macro creates controller's endpoint
     */
   private:
    OATPP_COMPONENT(std::shared_ptr<Users>, users);  // Get users component
    using Uuid = oatpp::postgresql::Uuid;

   public:
    UserController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper) {
    }

    static std::shared_ptr<UserController> createShared(
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)  // Inject objectMapper component here as default parameter
    ) {
        return std::make_shared<UserController>(objectMapper);
    }
    ENDPOINT_INFO(createUser) {
        info->summary = "Create new user";
        info->description = "Create new user with specified username, email and password";

        info->body.addExample("Example user", (oatpp::String) "{\n  \"username\":\"johnDoe123\",\n  \"email\":\"johnDoe123@gmail.com\",\n  \"password\":\"password\"\n}");
        info->addTag("Users");
        info->body.name = "User data";
        info->addConsumes<Object<UserDTO>>("application/json");

        info->addResponse<Object<AuthDTO>>(Status::CODE_201, "application/json", "User created");
        info->addResponse<Object<StatusDTO>>(Status::CODE_400, "application/json", "Invalid Email");
        info->addResponse<Object<StatusDTO>>(Status::CODE_409, "application/json", "User with provided username or email already exists");
    }
    ENDPOINT("POST", "/api/users/signup", createUser,
             BODY_DTO(Object<UserDTO>, user)) {
        return createDtoResponse(Status::CODE_201, users->CreateUser(user));
    }

    ENDPOINT_INFO(validateUserLogin) {
        info->summary = "Validate user login";
        info->description = "Validate user password and email and send cookies";

        info->body.addExample("Example user", (oatpp::String) "{\n  \"username\":\"johnDoe123\",\n  \"email\":\"johnDoe123@gmail.com\",\n  \"password\":\"password\"\n}");
        info->addTag("Users");

        info->addResponse<Object<UserDTO>>(Status::CODE_200, "application/json", "Success");
        info->addResponse<Object<StatusDTO>>(Status::CODE_401, "application/json", "Wrong password or username");
    }
    ENDPOINT("POST", "/api/users/login", validateUserLogin,
             BODY_DTO(Object<UserDTO>, user)) {
        return createDtoResponse(Status::CODE_200, users->validateLogin(user));
    }

    ENDPOINT_INFO(updateUser) {
        info->summary = "Update user";
        info->description = "Update one user by their Identifier";

        info->addTag("Users");

        info->addConsumes<Object<UserDTO>>("application/json", "User data");

        info->addResponse<Object<UserDTO>>(Status::CODE_200, "application/json");
        info->addResponse<Object<StatusDTO>>(Status::CODE_404, "application/json");
        info->addResponse<Object<StatusDTO>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("PUT", "/api/users/update", updateUser,
             BODY_DTO(Object<UserDTO>, user),
             BUNDLE(Uuid, userId)) {
        user->id = userId->toString();
        return createDtoResponse(Status::CODE_200, users->updateUser(user));
    }
    ENDPOINT_INFO(verifyToken) {
        info->summary = "Verify token";
        info->description = "Verify token and return user id";

        info->addTag("Auth");

        info->addResponse<String>(Status::CODE_200, "text/plain", "Success");
        info->addResponse<Object<StatusDTO>>(Status::CODE_401, "application/json", "Invalid token");
    }
    ENDPOINT("GET", "/api/auth/verifyToken", verifyToken,
             BUNDLE(Uuid, userId)) {
        return createResponse(Status::CODE_200, userId->toString());
    }
};

#include OATPP_CODEGEN_END(ApiController)  //<- End Codegen

#endif /* UserController_hpp */