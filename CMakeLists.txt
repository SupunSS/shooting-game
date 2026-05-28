cmake_minimum_required(VERSION 3.16)
project(ShooterGame)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "C:/lib/SFML/lib/cmake/SFML")

find_package(SFML 3 COMPONENTS System Window Graphics Audio REQUIRED)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

if(NOT SOURCES)
    message(WARNING "No source files found with GLOB_RECURSE; falling back to explicit src/main.cpp")
    list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")
endif()

message(STATUS "Collected source files: ${SOURCES}")

add_executable(ShooterGame ${SOURCES})

target_link_libraries(ShooterGame SFML::System SFML::Window SFML::Graphics SFML::Audio)

add_custom_command(TARGET ShooterGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "C:/lib/SFML/bin" $<TARGET_FILE_DIR:ShooterGame>)

# Copy project assets to the executable output so runtime can load them via relative paths
add_custom_command(TARGET ShooterGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets" $<TARGET_FILE_DIR:ShooterGame>/assets)
