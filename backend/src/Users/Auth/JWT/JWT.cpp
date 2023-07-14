#include "JWT.hpp"

JWT::JWT(const oatpp::String& secret,
         const oatpp::String& issuer,
         const oatpp::UInt32& expTime)
    : m_secret(secret),
      m_issuer(issuer),
      m_verifier(jwt::verify()
                     .allow_algorithm(jwt::algorithm::hs256{secret})
                     .with_issuer(issuer)) {
    this->m_expTime = std::chrono::seconds(expTime);
}

oatpp::String JWT::createToken(const std::shared_ptr<Payload>& payload) {
    if (!payload->userId) {
        return nullptr;
    }
    auto now = std::chrono::system_clock::now();
    auto tokenExpirationTime = now + m_expTime;
    auto token = jwt::create()
                     .set_issuer(m_issuer)
                     .set_type("JWS")
                     .set_payload_claim("userId", jwt::claim(picojson::value(payload->userId->toString())))
                     .set_expires_at(tokenExpirationTime)
                     .set_issued_at(now)
                     .sign(jwt::algorithm::hs256(this->m_secret));
    return token;
}

std::shared_ptr<JWT::Payload> JWT::readAndVerifyToken(const oatpp::String& token) {
    auto decoded = jwt::decode(token);
    m_verifier.verify(decoded);

    auto payload = std::make_shared<JWT::Payload>();

    payload->userId = (oatpp::String)decoded.get_payload_json().at("userId").to_str();

    return payload;
}