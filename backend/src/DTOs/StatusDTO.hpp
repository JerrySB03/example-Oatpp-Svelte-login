#ifndef StatusDTO_hpp
#define StatusDTO_hpp

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class StatusDTO : public oatpp::DTO {
    DTO_INIT(StatusDTO, DTO)

    DTO_FIELD_INFO(status) { info->description = "Short status text"; }
    DTO_FIELD_INFO(code) { info->description = "Status code"; }
    DTO_FIELD_INFO(message) { info->description = "Verbose message"; }

    DTO_FIELD(String, status);
    DTO_FIELD(Int32, code);
    DTO_FIELD(String, message);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* StatusDTO_hpp */