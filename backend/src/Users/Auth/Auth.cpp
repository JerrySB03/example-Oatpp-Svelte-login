#include "Auth.hpp"

AuthHandler::AuthHandler()
    : oatpp::web::server::handler::BearerAuthorizationHandler("Example login API" /* Realm */) {}

std::shared_ptr<AuthHandler::AuthorizationObject> AuthHandler::authorize(const oatpp::String& token) {
    return jwt->readAndVerifyToken(token);
}