#include "ErrorHandler.hpp"

ErrorHandler::ErrorHandler(const std::shared_ptr<oatpp::data::mapping::ObjectMapper> &objectMapper)
    : m_objectMapper(objectMapper) {
}

std::shared_ptr<ErrorHandler::OutgoingResponse>
ErrorHandler::handleError(const Status &status, const oatpp::String &message, const Headers &headers) {
    auto error = StatusDTO::createShared();
    auto outStatus = Status(status);

    error->message = message;

    if (outStatus.code >= 500) {
        OATPP_LOGE("HTTP Code " + std::to_string(status.code), message->c_str());  // Log the error
        if (error->message->find("DATABASE-ERROR") != std::string::npos) {
            error->message = "Internal server error";
            error->status = "Database error";

        } else if (error->status == nullptr) {
            error->status = "Error";
        }
    }
    error->code = outStatus.code;
    auto response = ResponseFactory::createResponse(outStatus, error, m_objectMapper);

    for (const auto &pair : headers.getAll()) {
        response->putHeader(pair.first.toString(), pair.second.toString());
    }

    return response;
}