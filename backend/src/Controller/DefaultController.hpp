#ifndef DefaultController_hpp
#define DefaultController_hpp

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "DTOs/DTOs.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)  // <- Begin Codegen

class DefaultController : public oatpp::web::server::api::ApiController {
    /*
     * ENDPOINT_INFO macro adds info about the endpoint to SwaggerUI documentation
     * ENDPOINT macro creates controller's endpoint
     */
   public:
    DefaultController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper) {
    }
    static std::shared_ptr<DefaultController> createShared(
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)  // Inject objectMapper component here as default parameter
    ) {
        return std::make_shared<DefaultController>(objectMapper);
    }
    ENDPOINT_INFO(apiRoot) {
        info->summary = "Test API endpoint";
        info->description = "Endpoint returning basic json data";

        info->addResponse<Object<HelloDTO>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/api/", apiRoot,
             HEADER(String, userAgent, "User-Agent"),
             HEADER(String, host, "Host"),
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        auto dto = HelloDTO::createShared();
        dto->message = "Api GET request is working!";
        dto->server = Header::Value::SERVER;
        dto->userAgent = userAgent;
        dto->host = host;
        const auto connectionProperties = request->getConnection()->getInputStreamContext().getProperties();
        dto->client = connectionProperties.get("peer_address");
        return createDtoResponse(Status::CODE_200, dto);
    }
    ENDPOINT_INFO(root) {
        info->summary = "Root redirect";
        info->description = "Root redirect endpoint to /api";

        info->hide = true;
    }
    ENDPOINT("GET", "/", root) {
        auto response = createResponse(Status::CODE_302, "");
        response->putHeader("Location", "/api");
        return response;
    }
};

#include OATPP_CODEGEN_END(ApiController)

#endif /* DefaultController_hpp */