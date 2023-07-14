add_executable(${project_name}-test
        test.cpp
)

target_link_libraries(${project_name}-test ${project_name}-core oatpp::oatpp-test)
add_dependencies(${project_name}-test ${project_name}-core oatpp::oatpp-test)

enable_testing()
add_test(NAME ${project_name}-user-tests COMMAND ${project_name}-test)