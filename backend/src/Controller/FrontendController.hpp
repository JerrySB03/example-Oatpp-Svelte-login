#ifndef FrontendController_hpp
#define FrontendController_hpp

#include <filesystem>
#include <fstream>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>
#include <sstream>
#include <iostream>

#include "DTOs/StatusDTO.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)  //<- Begin Codegen

/**
 * Frontend server controller.
 */
class FrontendController : public oatpp::web::server::api::ApiController {
   public:
    FrontendController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper) {
    }

    static std::shared_ptr<FrontendController> createShared(
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)  // Inject objectMapper component here as default parameter
    ) {
        return std::make_shared<FrontendController>(objectMapper);
    }
    ENDPOINT("GET", "/", root) {
        auto indexFile = oatpp::String::loadFromFile(FRONTEND_BUILD_DIR "/index.html");
        return createResponse(Status::CODE_200, indexFile);
    }
    ENDPOINT("GET", "*", staticFilesEndpoint,
             REQUEST(std::shared_ptr<IncomingRequest>, request)) {
        // Get the requested file path from the URI
        auto filePath = request->getPathTail();
        OATPP_ASSERT_HTTP(filePath, Status::CODE_400, "Empty filename");

        filePath = FRONTEND_BUILD_DIR + filePath;
        // Open the file
        std::ifstream file(filePath->c_str(), std::ios::in);
        std::stringstream fileStream;
        fileStream << file.rdbuf();

        // Check if the file exists
        if (fileStream.str().length() > 0) {
            return createResponse(Status::CODE_200, fileStream.str());
        } else if (std::filesystem::is_directory(filePath->c_str())) {
            filePath = filePath + ".html";
            fileStream = std::stringstream();
            file = std::ifstream((filePath)->c_str(), std::ios::in);
            fileStream << file.rdbuf();
            if (fileStream.str().length() > 0) {
                return createResponse(Status::CODE_200, fileStream.str());
            }else{
                std::cout << "File not found: " << filePath->c_str() << ":\n" <<fileStream.str() << std::endl;
                return createResponse(Status::CODE_404, "File not found!");
            }
        } else {
            // Return a 404 Not Found response
            return createResponse(Status::CODE_404, "File not found");
        }
    }
};

#include OATPP_CODEGEN_END(ApiController)  //<- End Codegen

#endif /* FrontendController_hpp */