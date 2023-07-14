#ifndef UserDTOs_hpp
#define UserDTOs_hpp

#define HASHLEN 64
#define SALTLEN 32

#include <oatpp-postgresql/mapping/type/Uuid.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class UserDTO : public oatpp::DTO {
    DTO_INIT(UserDTO, DTO)

    DTO_FIELD_INFO(id) { info->description = "User identifier"; }
    DTO_FIELD_INFO(username) { info->description = "username"; }
    DTO_FIELD_INFO(email) { info->description = "User email"; }
    DTO_FIELD_INFO(password) { info->description = "User password"; }

    DTO_FIELD(String, id);
    DTO_FIELD(String, username, "username");
    DTO_FIELD(String, email, "email");
    DTO_FIELD(String, password, "password");
};

class AuthDTO : public oatpp::DTO {
    DTO_INIT(AuthDTO, DTO)

    DTO_FIELD(String, token);
};
#include OATPP_CODEGEN_END(DTO)

#endif /* UserDTOs_hpp */