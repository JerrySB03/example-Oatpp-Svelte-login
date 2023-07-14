#ifndef ConfigDTO_hpp
#define ConfigDTO_hpp

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class DatabaseConfigDTO : public oatpp::DTO {
    DTO_INIT(DatabaseConfigDTO, DTO)

    DTO_FIELD(String, connectionUrl, "connectionUrl");
    DTO_FIELD(UInt16, maxConnections, "maxConnections");
    DTO_FIELD(String, migrationPath, "migrationPath");
};
class LogConfigDTO : public oatpp::DTO {
    DTO_INIT(LogConfigDTO, DTO)

    DTO_FIELD(String, logFilePath, "filePath");
    DTO_FIELD(Boolean, debug, "D");
    DTO_FIELD(Boolean, error, "E");
    DTO_FIELD(Boolean, info, "I");
    DTO_FIELD(Boolean, verbose, "V");
    DTO_FIELD(Boolean, warning, "W");
};
class HashConfigDTO : public oatpp::DTO {
    DTO_INIT(HashConfigDTO, DTO)

    DTO_FIELD(UInt8, t_cost, "iterations");
    DTO_FIELD(UInt32, m_cost, "memory");
    DTO_FIELD(UInt8, parallelism, "parallelism");
    DTO_FIELD(UInt8, saltLength, "saltlength");
    DTO_FIELD(UInt8, hashLength, "hashlength");
};
class JWTConfigDTO : public oatpp::DTO {
    DTO_INIT(JWTConfigDTO, DTO)

    DTO_FIELD(String, secret, "secret");
    DTO_FIELD(String, issuer, "issuer");
    DTO_FIELD(UInt32, expiry, "expiry");
};
class ConfigDTO : public oatpp::DTO {
    DTO_INIT(ConfigDTO, DTO)

    DTO_FIELD(String, baseFolder, "folder");
    DTO_FIELD(UInt16, port, "port");
    DTO_FIELD(Object<DatabaseConfigDTO>, database, "database");
    DTO_FIELD(Object<LogConfigDTO>, logConf, "logs");
    DTO_FIELD(Object<HashConfigDTO>, hashConf, "hashing");
    DTO_FIELD(Object<JWTConfigDTO>, jwtConf, "jwt");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* ConfigDTO_hpp */