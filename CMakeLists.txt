cmake_minimum_required(VERSION 3.14.0)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Configuring the source files to generate the executable
include_directories(src)
file(GLOB_RECURSE SOURCES src/*.cpp)
if(WIN32)
    set(SOURCES ${SOURCES} src/resource.rc)
    add_executable(game WIN32 ${SOURCES})
else()
    add_executable(game ${SOURCES})
endif()
target_compile_features(game PRIVATE cxx_std_17)

find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)
target_link_libraries(game PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(game PRIVATE nlohmann_json::nlohmann_json)

find_package(Clipper2 CONFIG REQUIRED)
target_link_libraries(game PRIVATE Clipper2::Clipper2)

find_package(poly2tri CONFIG REQUIRED)
target_link_libraries(game PRIVATE poly2tri::poly2tri)

# Specify the asset directories
set(ASSET_DIR ${CMAKE_SOURCE_DIR}/assets)

# Copy assets to the build directory
add_custom_command(
    TARGET game PRE_BUILD 
    COMMENT "Copy Assets"
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:game>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${ASSET_DIR} $<TARGET_FILE_DIR:game>/assets
)

# CPack configuration
if(WIN32)
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
endif()
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "Snake")
set(CPACK_PACKAGE_VERSION "0.2")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PACKAGE_VENDOR "xinchengo @ candyx")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Eat all the fish and avoid green circles")
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/assets/images/icon.ico")
set(CPACK_NSIS_DISPLAY_NAME "Twisty Snake & Wavy Fish")
set(CPACK_NSIS_HELP_LINK "https://github.com/xinchengo/GameDemo")
set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/xinchengo/GameDemo")
set(CPACK_NSIS_CONTACT "2773315798@qq.com")
set(CPACK_NSIS_MODIFY_PATH OFF)
set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
set(CPACK_NSIS_IGNORE_LICENSE_PAGE ON)
set(CPACK_GENERATOR "ZIP;NSIS")

set(CPACK_NSIS_CREATE_ICONS_EXTRA "
  CreateShortCut '\\\$SMPROGRAMS\\\\\\\$STARTMENU_FOLDER\\\\Twisty Snake & Wavy Fish.lnk' '\\\$INSTDIR\\\\game.exe'
  CreateShortCut '\\\$SMPROGRAMS\\\\\\\$STARTMENU_FOLDER\\\\Uninstall Twisty Snake & Wavy Fish.lnk' '\\\$INSTDIR\\\\Uninstall.exe'
")

set(CPACK_NSIS_DELETE_ICONS_EXTRA "
  Delete '\\\$SMPROGRAMS\\\\\\\$STARTMENU_FOLDER\\\\Twisty Snake & Wavy Fish.lnk'
  Delete '\\\$SMPROGRAMS\\\\\\\$STARTMENU_FOLDER\\\\Uninstall Twisty Snake & Wavy Fish.lnk'
")

install(TARGETS game DESTINATION .)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION .)

include(CPack)