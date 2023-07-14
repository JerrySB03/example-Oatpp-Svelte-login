#ifndef DTOs_hpp
#define DTOs_hpp

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class HelloDTO : public oatpp::DTO {
    DTO_INIT(HelloDTO, DTO)

    DTO_FIELD_INFO(message) { info->description = "Message"; }
    DTO_FIELD_INFO(userAgent) { info->description = "User agent"; }
    DTO_FIELD_INFO(server) { info->description = "Server name and version"; }
    DTO_FIELD_INFO(host) { info->description = "Server IP address and port"; }
    DTO_FIELD_INFO(client) { info->description = "Client IP address"; }

    DTO_FIELD(String, message);
    DTO_FIELD(String, userAgent, "user-agent");
    DTO_FIELD(String, host);
    DTO_FIELD(String, server);
    DTO_FIELD(String, client);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */