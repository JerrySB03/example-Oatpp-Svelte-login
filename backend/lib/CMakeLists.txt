set(JWT_BUILD_EXAMPLES OFF)
add_subdirectory(jwt-cpp)
target_include_directories(${project_name}-core PRIVATE jwt-cpp/include)