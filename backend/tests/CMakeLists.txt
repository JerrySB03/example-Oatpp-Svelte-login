add_executable(${PROJECT_NAME}-test
        test.cpp
)

target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME}-core oatpp::oatpp-test)
add_dependencies(${PROJECT_NAME}-test ${PROJECT_NAME}-core oatpp::oatpp-test)

enable_testing()
add_test(NAME ${PROJECT_NAME}-user-tests COMMAND ${PROJECT_NAME}-test)