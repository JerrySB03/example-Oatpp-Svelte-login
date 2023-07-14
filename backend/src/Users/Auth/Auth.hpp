#ifndef Auth_hpp
#define Auth_hpp

#include <oatpp/core/macro/component.hpp>

#include "JWT/JWT.hpp"

class AuthHandler : public oatpp::web::server::handler::BearerAuthorizationHandler {
   private:
    OATPP_COMPONENT(std::shared_ptr<JWT>, jwt);

   public:
    AuthHandler();

    std::shared_ptr<AuthorizationObject> authorize(const oatpp::String& token) override;
};

#endif /* Auth_hpp */