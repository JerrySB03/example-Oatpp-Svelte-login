cmake_minimum_required(VERSION 3.11)

project(oatpp-svelte-example-login CXX)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Find dependencies
find_package(oatpp 1.3.0 REQUIRED)
find_package(oatpp-postgresql 1.3.0 REQUIRED)
find_package(oatpp-swagger 1.3.0 REQUIRED)

add_definitions(
        ## define path to swagger-ui static resources folder
        -DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res"

        ## Path to database migration scripts
        -DDATABASE_MIGRATIONS="${CMAKE_CURRENT_SOURCE_DIR}/sql"

        ## Cmake build type
        -DCMAKE_BUILD_TYPE=DEBUG

        ## Postgres server url
        -DPOSTGRESQL_URL="postgresql://username:password@localhost:5432/oatpp_svelte_example_login"

        ## Postgres server max connections, must be specified in the database
        -DPOSTGRESQL_MAX_CONN=1024
)
include(CTest)

add_subdirectory(tests)
add_subdirectory(src)
add_subdirectory(lib)

set_target_properties(${project_name}-core ${project_name}-exe ${project_name}-test PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)
