set(VERSION_MAJOR "1" CACHE INTERNAL "Major version")
set(VERSION_MINOR "0" CACHE INTERNAL "Minor version")
set(VERSION_PATCH "1" CACHE INTERNAL "Patch version")

set(VERSION_STRVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set(FINAL_APPLICATION_NAME "Hellow World App")

set(PROJECT_VESRION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

project(helloworld VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

add_executable(helloworld_cli main.cpp)
add_library(helloworld_lib lib.cpp)
add_executable(test_version test_version.cpp)

set_target_properties(helloworld_cli helloworld_lib test_version PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(helloworld_lib
    PRIVATE "${CMAKE_BINARY_DIR}/mod_1/hw_1"
)

target_include_directories(test_version
    PRIVATE "${CMAKE_BINARY_DIR}/mod_1/hw_1"
)

set_target_properties(test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/3rd/googletest
)

target_link_libraries(helloworld_cli PRIVATE
    helloworld_lib
)

target_link_libraries(test_version PRIVATE
    gtest gmock gtest_main helloworld_lib
)

target_compile_options(helloworld_cli PRIVATE
    -Wall -Wextra -pedantic -Werror
)
target_compile_options(helloworld_lib PRIVATE
    -Wall -Wextra -pedantic -Werror
)
target_compile_options(test_version PRIVATE
    -Wall -Wextra -pedantic -Werror
)

install(TARGETS helloworld_cli RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/mod_1/hw_1/bin)
install(TARGETS test_version RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/mod_1/hw_1/bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_NAME helloworld)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT dmitriy.rybalkin@gmail.com)

include(CPack)

enable_testing()

add_test(test_version ${CMAKE_BINARY_DIR}/mod_1/hw_1/test_version)