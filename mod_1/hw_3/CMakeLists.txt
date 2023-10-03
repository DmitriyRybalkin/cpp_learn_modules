
project(ip_filter)

project(allocators)

add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME}
    PRIVATE "${CMAKE_BINARY_DIR}/mod_1/hw_3"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -pedantic -Werror
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/mod_1/hw_3/bin)