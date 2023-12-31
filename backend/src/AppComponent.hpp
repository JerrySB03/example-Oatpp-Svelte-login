#ifndef AppComponent_hpp
#define AppComponent_hpp

#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/core/utils/ConversionUtils.hpp>
#include <oatpp/network/ConnectionPool.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/web/server/interceptor/AllowCorsGlobal.hpp>

#include "Database/DatabaseComponent.hpp"
#include "ErrorHandler/ErrorHandler.hpp"
#include "SwaggerComponent.hpp"
#include "Users/Auth/Interceptor/Interceptor.hpp"
#include "Users/UsersComponent.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
   public:
   private:
    oatpp::base::CommandLineArguments m_cmdArgs;

   public:
    /**
     * Swagger component
     */
    SwaggerComponent swaggerComponent;
    // Get command line arguments
    AppComponent(const oatpp::base::CommandLineArguments &cmdArgs)
        : m_cmdArgs(cmdArgs) {
    }
    /**
     * Database component
     */
    DatabaseComponent databaseComponent;
    UsersComponent usersComponent;

    /**
     * Create ObjectMapper component to serialize/deserialize DTOs in Controller's API
     */

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)
    ([] {
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    objectMapper->getDeserializer()->getConfig()->allowUnknownFields = false;
    return objectMapper; }());

    /**
     *  Create ConnectionProvider component which listens on the port
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)
    ([this] {
    v_int32 port = oatpp::utils::conversion::strToInt32(m_cmdArgs.getNamedArgumentValue("--port", "8000" /* default value */));
    
    if((v_uint16) port != port || port == 0){
            OATPP_LOGE("Port", "Invalid port entered");
            exit(EXIT_FAILURE);
    }
    
    return oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", (v_uint16)port, oatpp::network::Address::IP_4}, true); }());

    /**
     *  Create Router component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)
    ([] { return oatpp::web::server::HttpRouter::createShared(); }());

    /**
     *  Create ConnectionHandler component which uses Router component to route requests
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)
    ([] {
     OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);           // get Router component
     OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper); // get ObjectMapper component

     auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
     connectionHandler->setErrorHandler(std::make_shared<ErrorHandler>(objectMapper));

     connectionHandler->addRequestInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowOptionsGlobal>());
     connectionHandler->addRequestInterceptor(std::make_shared<AuthInterceptor>());

     return connectionHandler; }());    
};

#endif /* AppComponent_hpp */