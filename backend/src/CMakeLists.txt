add_library(${project_name}-core
        Controller/UserController.hpp
        Controller/DefaultController.hpp
        AppComponent.hpp
        SwaggerComponent.hpp
        Users/Users.cpp
        Users/Users.hpp
        Users/Auth/Auth.hpp
        Users/Auth/Auth.cpp
        Users/Auth/JWT/JWT.hpp
        Users/Auth/JWT/JWT.cpp
        Users/Auth/Interceptor/Interceptor.hpp
        Users/Auth/Interceptor/Interceptor.cpp
        Users/UsersComponent.hpp
        ErrorHandler/ErrorHandler.hpp
        ErrorHandler/ErrorHandler.cpp
        DTOs/DTOs.hpp
        DTOs/UserDTOs.hpp
        DTOs/StatusDTO.hpp
        DTOs/ConfigDTO.hpp
        Database/Models/UserModel.hpp
        Database/DatabaseComponent.hpp
        Database/UserDb.hpp
)

add_executable(${project_name}-exe
    App.cpp    
)

# Link the library to oatpp
target_link_libraries(${project_name}-core
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-postgresql
        PUBLIC oatpp::oatpp-swagger
        PUBLIC argon2
        PUBLIC jwt-cpp::jwt-cpp
)

target_include_directories(${project_name}-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${project_name}-exe ${project_name}-core) 
add_dependencies(${project_name}-exe ${project_name}-core)

include_directories(${OATPP_INCLUDE_DIRS})