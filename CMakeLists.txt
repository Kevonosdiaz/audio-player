cmake_minimum_required(VERSION 3.19)
project(audio-player LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MPDCLIENT REQUIRED libmpdclient)

qt_standard_project_setup()

qt_add_executable(audio-player
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(audio-player
    PRIVATE
        Qt::Core
        Qt::Widgets
        ${MPDCLIENT_LIBRARIES}
)

target_include_directories(audio-player
    PRIVATE ${MPDCLIENT_INCLUDE_DIRS}
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
