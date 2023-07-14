#ifndef JWT_hpp
#define JWT_hpp

#include <jwt-cpp/jwt.h>

#include <oatpp/core/Types.hpp>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>
#include <oatpp-postgresql/Types.hpp>

class JWT {
   public:
    struct Payload : public oatpp::web::server::handler::AuthorizationObject {
        oatpp::postgresql::Uuid userId;
    };

   private:
    oatpp::String m_secret;
    oatpp::String m_issuer;
    std::chrono::seconds m_expTime;
    jwt::verifier<jwt::default_clock, jwt::traits::kazuho_picojson> m_verifier;

   public:
    JWT(const oatpp::String& secret,
        const oatpp::String& issuer,
        const oatpp::UInt32& expTime);

    oatpp::String createToken(const std::shared_ptr<Payload>& payload);

    std::shared_ptr<Payload> readAndVerifyToken(const oatpp::String& token);
};

#endif /* JWT_hpp */