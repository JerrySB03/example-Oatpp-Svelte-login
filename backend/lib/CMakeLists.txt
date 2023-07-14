set(JWT_BUILD_EXAMPLES OFF)
add_subdirectory(jwt-cpp)
target_include_directories(${PROJECT_NAME}-core PRIVATE jwt-cpp/include)