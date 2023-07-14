add_library(${PROJECT_NAME}-core
        Controller/UserController.hpp
        Controller/DefaultController.hpp
        Controller/FrontendController.hpp
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
        Database/Models/UserModel.hpp
        Database/DatabaseComponent.hpp
        Database/UserDb.hpp
        # frontend/frontend.hpp
        # frontend/frontend.cpp
)

add_executable(${PROJECT_NAME}-exe
    App.cpp    
)

# Link the library to oatpp
target_link_libraries(${PROJECT_NAME}-core
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-postgresql
        PUBLIC oatpp::oatpp-swagger
        PUBLIC argon2
        PUBLIC jwt-cpp::jwt-cpp
)

target_include_directories(${PROJECT_NAME}-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}-exe ${PROJECT_NAME}-core) 
add_dependencies(${PROJECT_NAME}-exe ${PROJECT_NAME}-core)

include_directories(${OATPP_INCLUDE_DIRS})