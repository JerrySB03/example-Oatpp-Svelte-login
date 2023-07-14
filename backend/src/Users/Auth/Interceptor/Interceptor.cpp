#include "Interceptor.hpp"

AuthInterceptor::AuthInterceptor() {
    authEndpoints.route("GET", "/*", false);
    authEndpoints.route("GET", "/api/*", true);

    authEndpoints.route("GET", "/api", false);
    authEndpoints.route("GET", "/", false);

    authEndpoints.route("POST", "/api/users/login", false);
    authEndpoints.route("POST", "/api/users/signup", false);

    authEndpoints.route("GET", "/swagger/*", false);
    authEndpoints.route("GET", "/api-docs/oas-3.0.0.json", false);
}

std::shared_ptr<AuthInterceptor::OutgoingResponse> AuthInterceptor::intercept(const std::shared_ptr<IncomingRequest>& request) {
    auto r = authEndpoints.getRoute(request->getStartingLine().method, request->getStartingLine().path);
    if (r && !r.getEndpoint()) {
        return nullptr;  // Continue without Authorization
    }
    auto authHeader = request->getHeader(oatpp::web::protocol::http::Header::AUTHORIZATION);
    std::shared_ptr<JWT::Payload> authObject;
    try {
        authObject = std::static_pointer_cast<JWT::Payload>(authHandler->handleAuthorization(authHeader));
    } catch (const std::exception& e) {
        throw oatpp::web::protocol::http::HttpError(oatpp::web::protocol::http::Status::CODE_401, e.what(), {});
    }

    if (authObject) {
        request->putBundleData("userId", authObject->userId);
        return nullptr;  // Continue - token is valid.
    }
    throw oatpp::web::protocol::http::HttpError(oatpp::web::protocol::http::Status::CODE_401, "Unauthorized", {});
}