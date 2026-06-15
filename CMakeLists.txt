cmake_minimum_required(VERSION 3.19)
project(audio-player LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(PkgConfig REQUIRED)
find_package(TagLib 2.0.0 REQUIRED)
pkg_check_modules(MPDCLIENT REQUIRED libmpdclient)

qt_standard_project_setup()

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mpdhandler.cpp
    src/tagart.cpp
)

set(HEADERS
    src/mainwindow.h
    src/mpdhandler.h
    src/tagart.h
)

set(UI
    src/mainwindow.ui
)

qt_add_executable(audio-player
    WIN32 MACOSX_BUNDLE
    ${SOURCES}
    ${HEADERS}
    ${UI}
    src/songinfo.h
    src/scalingpixmaplabel.h src/scalingpixmaplabel.cpp
)

target_link_libraries(audio-player
    PRIVATE
        Qt::Core
        Qt::Widgets
        ${MPDCLIENT_LIBRARIES}
        TagLib::tag
)

target_include_directories(audio-player
    PRIVATE ${MPDCLIENT_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS audio-player
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET audio-player
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
