cmake_minimum_required(VERSION 3.14.0)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Importing the SFML library

set(ONLINE https://github.com/SFML/SFML.git)
set(LOCAL ${CMAKE_SOURCE_DIR}/SFML)

include(FetchContent)
FetchContent_Declare(SFML
    # GIT_REPOSITORY ${ONLINE}
    # GIT_TAG 2.6.x
    # GIT_SHALLOW ON
    SOURCE_DIR ${LOCAL}
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Configuring the source files to generate the executable

file(GLOB_RECURSE SOURCES src/*.cpp)
# set(EXCLUDED_FILE ${CMAKE_SOURCE_DIR}/src/fishTrainer.cpp)  
# list(REMOVE_ITEM SOURCES ${EXCLUDED_FILE})

include_directories(src)
add_executable(game ${SOURCES})
target_link_libraries(game PRIVATE sfml-graphics)
target_compile_features(game PRIVATE cxx_std_17)

# Specify the asset directories
set(ASSET_DIR ${CMAKE_SOURCE_DIR}/assets)
set(IMAGE_DIR ${ASSET_DIR}/images)

# Copy assets to the build directory

file(GLOB_RECURSE IMAGES "${IMAGE_DIR}/*") 
add_custom_command(TARGET game POST_BUILD  
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:game>/assets/images  
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${IMAGES} $<TARGET_FILE_DIR:game>/assets/images
)

if(WIN32)
    add_custom_command(
        TARGET game
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:game>
        VERBATIM)
endif()