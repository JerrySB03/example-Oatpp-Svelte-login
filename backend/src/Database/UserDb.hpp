#ifndef UserDB_hpp
#define UserDB_hpp

#include <oatpp-postgresql/orm.hpp>

#include "DTOs/UserDTOs.hpp"
#include "Models/UserModel.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient)

class UserDb : public oatpp::orm::DbClient {
   public:
    UserDb(const std::shared_ptr<oatpp::orm::Executor> &executor)
        : oatpp::orm::DbClient(executor) {
        oatpp::orm::SchemaMigration migration(executor, "Users");
        migration.addFile(1, DATABASE_MIGRATIONS "/Users/001_init.sql");
        migration.migrate();

        auto version = executor->getSchemaVersion();
        OATPP_LOGD("UserDb", "Migration - OK. Version=%lld.", version);
    }
    QUERY(createUser,
          "INSERT INTO Users "
          " (username, email, hash) VALUES "
          " (:user.username, :user.email, :user.hash) "
          "RETURNING id;",
          PREPARE(true),
          PARAM(oatpp::Object<UserModel>, user))

    QUERY(updateUser,
          "UPDATE Users "
          "SET "
          " username=:user.username, "
          " email=:user.email, "
          " hash=:user.hash "
          "WHERE "
          " id=:user.id;",
          PREPARE(true),
          PARAM(oatpp::Object<UserModel>, user))

    QUERY(getUserById,
          "SELECT "
          " * "
          "FROM Users "
          "WHERE id=:id;",
          PREPARE(true),
          PARAM(oatpp::postgresql::Uuid, id))

    QUERY(getUserId,
          "SELECT id FROM "
          "Users WHERE "
          " username=:user.username "
          "OR "
          " email=:user.email;",
          PARAM(oatpp::Object<UserModel>, user))
    QUERY(deleteUserById,
          "DELETE FROM Users WHERE id=:id;",
          PREPARE(true),
          PARAM(oatpp::postgresql::Uuid, id))
    QUERY(getHashData,
          "SELECT"
          " hash "
          "FROM "
          " Users "
          "WHERE "
          " id=:id;",
          PREPARE(true),
          PARAM(oatpp::postgresql::Uuid, id))
    QUERY(getUserByUsername,
          "SELECT * FROM Users WHERE username=:username;",
          PREPARE(true),
          PARAM(oatpp::String, username))
    QUERY(getUserByEmail,
          "SELECT * FROM Users WHERE email=:email;",
          PREPARE(true),
          PARAM(oatpp::String, email))
};

#include OATPP_CODEGEN_END(DbClient)

#endif /* UserDB_hpp */