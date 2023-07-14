#ifndef UsersComponent_hpp
#define UsersComponent_hpp

#include <oatpp/core/macro/component.hpp>

#include "Auth/Auth.hpp"
#include "Users.hpp"

class UsersComponent {
   private:
    using Status = oatpp::web::protocol::http::Status;

   public:
    OATPP_CREATE_COMPONENT(std::shared_ptr<JWT>, jwt)
    ([] {
        return std::make_shared<JWT>("TODO: RANDOM GENERATION AND SAVE TO ENV FILE",  // secret
                                     "example-Oatpp-Svelte-login",                    // issuer
                                     2592000                                          // 30 days
        );
    }());
    OATPP_CREATE_COMPONENT(std::shared_ptr<Users>, users)
    ([] {
        return std::make_shared<Users>();
    }());
    OATPP_CREATE_COMPONENT(std::shared_ptr<AuthHandler>, authHandler)
    ([this] {
        return std::make_shared<AuthHandler>();
    }());
};

#endif /* UsersComponent_hpp */