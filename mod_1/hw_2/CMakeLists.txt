
project(ip_filter)

set(PROJECT_TEST test_ip_filter)

add_executable(${PROJECT_NAME} ip_filter.cpp)
add_executable(${PROJECT_TEST} test_ip_filter.cpp)

set_target_properties(${PROJECT_NAME} ${PROJECT_TEST} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME}
    PRIVATE "${CMAKE_BINARY_DIR}/mod_1/hw_2"
)

target_include_directories(${PROJECT_TEST}
    PRIVATE "${CMAKE_BINARY_DIR}/mod_1/hw_2"
)

set_target_properties(${PROJECT_TEST} PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/3rd/googletest
)

target_link_libraries(${PROJECT_TEST} PRIVATE
    gtest gmock gtest_main
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -pedantic -Werror
)

target_compile_options(${PROJECT_TEST} PRIVATE
    -Wall -Wextra -pedantic -Werror
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/mod_1/hw_2/bin)
install(TARGETS ${PROJECT_TEST} RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/mod_1/hw_2/bin)

install(FILES files/ip_filter.tsv DESTINATION ${CMAKE_BINARY_DIR}/mod_1/hw_2/bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")

set(CPACK_PACKAGE_CONTACT dmitriy.rybalkin@gmail.com)

include(CPack)

enable_testing()

add_test(${PROJECT_TEST} ${CMAKE_BINARY_DIR}/mod_1/hw_2/${PROJECT_TEST})