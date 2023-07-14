
#ifndef AuthInterceptor_HPP
#define AuthInterceptor_HPP

#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>
#include <oatpp/web/server/interceptor/RequestInterceptor.hpp>
#include <oatpp/core/macro/component.hpp>

#include "Users/Auth/Auth.hpp"

class AuthInterceptor : public oatpp::web::server::interceptor::RequestInterceptor {
   private:
    OATPP_COMPONENT(std::shared_ptr<AuthHandler>, authHandler);

    oatpp::web::server::HttpRouterTemplate<bool> authEndpoints;

    using Status = oatpp::web::protocol::http::Status;

   public:
    AuthInterceptor();

    std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request) override;
};

#endif /* AuthInterceptor_HPP */