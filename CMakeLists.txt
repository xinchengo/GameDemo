cmake_minimum_required(VERSION 3.14.0)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)


# Configuring the source files to generate the executable

include_directories(src)
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(game ${SOURCES})
target_compile_features(game PRIVATE cxx_std_17)

include(FetchContent)

# Use FetchContent to import the SFML library
FetchContent_Declare(
    SFML
    # GIT_REPOSITORY https://github.com/SFML/SFML.git
    # GIT_TAG 2.6.x
    # GIT_SHALLOW ON
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/SFML
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)
target_link_libraries(game PRIVATE sfml-graphics)

# Use FetchContent to import the JSON library
FetchContent_Declare(
    json
    # GIT_REPOSITORY https://github.com/nlohmann/json.git
    # GIT_TAG 3.11.x
    # GIT_SHALLOW ON
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/json
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(json)
target_link_libraries(game PRIVATE nlohmann_json::nlohmann_json)


# Specify the asset directories
set(ASSET_DIR ${CMAKE_SOURCE_DIR}/assets)

# Copy assets to the build directory
add_custom_command(
    TARGET game PRE_BUILD 
    COMMENT "Copy Assets"
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:game>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${ASSET_DIR} $<TARGET_FILE_DIR:game>/assets
)

if(WIN32)
    add_custom_command(
        TARGET game
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:game>
        VERBATIM)
endif()