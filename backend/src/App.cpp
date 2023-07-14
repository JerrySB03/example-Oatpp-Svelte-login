#include <iostream>
#include <oatpp-swagger/Controller.hpp>
#include <oatpp/core/utils/ConversionUtils.hpp>
#include <oatpp/network/Server.hpp>

#include "AppComponent.hpp"
#include "Controller/DefaultController.hpp"
#include "Controller/UserController.hpp"
#include "Controller/FrontendController.hpp"

/**
 *  run() method.
 *  1) set Environment components.
 *  2) add ApiController's endpoints to router
 *  3) run server
 */
void serverStop() {
    std::cout << "Press Enter to stop the server" << std::endl;
    std::cin.ignore();
}
void run(const oatpp::base::CommandLineArguments& args) {
    AppComponent components(args);  // Create scope Environment components

    /* create ApiControllers and add endpoints to router */
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    oatpp::web::server::api::Endpoints docEndpoints;  // Register endpoints for API documentation (swagger-ui)

    router->addController(DefaultController::createShared());
    docEndpoints.append(router->addController(UserController::createShared())->getEndpoints());
    router->addController(oatpp::swagger::Controller::createShared(docEndpoints));  // Register swagger-ui endpoint
    router->addController(FrontendController::createShared()); // Register frontend controller, which serves static files - has lowest priority

    // create the server object
    oatpp::network::Server server(components.serverConnectionProvider.getObject(),
                                  components.serverConnectionHandler.getObject());

    std::thread serverThread([&server] {
        /* Run server */
        server.run();
    });
    OATPP_LOGI("Server", "Running on port %s...", components.serverConnectionProvider.getObject()->getProperty("port").toString()->c_str());
    serverStop();

    // Now, check if server is still running and stop it if needed
    if (server.getStatus() == oatpp::network::Server::STATUS_RUNNING) {
        server.stop();
    }

    // Finally, stop the ConnectionHandler and wait until all running connections are closed
    components.serverConnectionHandler.getObject()->stop();

    // Before returning, check if the server-thread has already stopped or if we need to wait for the server to stop
    if (serverThread.joinable()) {
        /* We need to wait until the thread is done */
        serverThread.join();
    } else {
        std::cout << "Server thread is not joinable" << std::endl;
    }
}

int main(int argc, const char* argv[]) {
    oatpp::base::Environment::init();
    run(oatpp::base::CommandLineArguments(argc, argv));

    /* Print how much objects were created during app running, and what have left-probably leaked */
    /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
    std::cout << "\nEnvironment:\n";
    std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
    std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";

    oatpp::base::Environment::destroy();

    return 0;
}