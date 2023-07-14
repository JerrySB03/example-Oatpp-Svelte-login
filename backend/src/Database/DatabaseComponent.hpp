#ifndef DatabaseComponent_hpp
#define DatabaseComponent_hpp

#include "UserDb.hpp"


class DatabaseComponent {
   public:
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::postgresql::ConnectionPool>, connectionPool)
    ([] {
        auto connectionProvider = std::make_shared<oatpp::postgresql::ConnectionProvider>(POSTGRESQL_URL);
        return oatpp::postgresql::ConnectionPool::createShared(connectionProvider,
            POSTGRESQL_MAX_CONN, // max connections
            std::chrono::seconds(5) /* Connection time to live*/); }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::postgresql::Executor>, databaseExecutor)
    ([] {
        /* get connection pool component */
        OATPP_COMPONENT(std::shared_ptr<oatpp::postgresql::ConnectionPool>, connectionPool);

        /* Create database-specific Executor */
        return std::make_shared<oatpp::postgresql::Executor>(connectionPool);
    }());


    OATPP_CREATE_COMPONENT(std::shared_ptr<UserDb>, userDb)
    ([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::postgresql::Executor>, databaseExecutor);
        // Get DB executor component and create a database client
        return std::make_shared<UserDb>(databaseExecutor);
    }());
};

#endif /* DatabaseComponent_hpp */