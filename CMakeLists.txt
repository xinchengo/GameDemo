cmake_minimum_required(VERSION 3.14.0)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)


# Configuring the source files to generate the executable

include_directories(src)
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(game ${SOURCES})
target_compile_features(game PRIVATE cxx_std_17)

find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)
target_link_libraries(game PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(game PRIVATE nlohmann_json::nlohmann_json)

find_package(Clipper2 CONFIG REQUIRED)
target_link_libraries(game PRIVATE Clipper2::Clipper2)

# Specify the asset directories
set(ASSET_DIR ${CMAKE_SOURCE_DIR}/assets)

# Copy assets to the build directory
add_custom_command(
    TARGET game PRE_BUILD 
    COMMENT "Copy Assets"
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:game>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${ASSET_DIR} $<TARGET_FILE_DIR:game>/assets
)