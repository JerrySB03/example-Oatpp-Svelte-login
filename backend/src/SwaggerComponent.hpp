#ifndef SwaggerComponent_hpp
#define SwaggerComponent_hpp

#include <oatpp-swagger/Model.hpp>
#include <oatpp-swagger/Resources.hpp>
#include <oatpp/core/macro/component.hpp>

/**
 *  Swagger ui is served at
 *  http://host:port/swagger/ui
 */
class SwaggerComponent {
   public:
    /**
     *  General API docs info
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)
    ([] {
        oatpp::swagger::DocumentInfo::Builder builder;

        builder
            .setTitle("Example oatpp-svelte login API")
            .setDescription("API example project with oatpp for backend and SvelteKit for frontend")
            .setVersion("1.0.0")
            .setContactName("Šimon Kimel")
            .setContactUrl("https://github.com/JerrySB03")
            .setLicenseName("Apache License, Version 2.0")
            .setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0")
            .addServer("http://localhost:8000", "Development server on localhost");

        return builder.build();
    }());

    /**
     *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)
    ([] {
        // Make sure to specify correct full path to oatpp-swagger/res folder !!!
        return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
    }());
};

#endif /* SwaggerComponent_hpp */