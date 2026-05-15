cmake_minimum_required(VERSION 3.19)
project(code LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(code
    WIN32 MACOSX_BUNDLE
    main.cpp
    janelaprincipal.cpp
    janelaprincipal.h
)

target_link_libraries(code
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS code
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET code
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
