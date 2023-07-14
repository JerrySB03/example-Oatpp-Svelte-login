#ifndef Database_User_Model_hpp
#define Database_User_Model_hpp

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp-postgresql/mapping/type/Uuid.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class UserModel : public oatpp::DTO {
    DTO_INIT(UserModel, DTO)
    
    DTO_FIELD(oatpp::postgresql::Uuid, id);
    DTO_FIELD(String, username);
    DTO_FIELD(String, email);
    DTO_FIELD(String, hash);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* Database_User_Model_hpp */