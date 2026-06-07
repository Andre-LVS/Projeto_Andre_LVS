cmake_minimum_required(VERSION 3.19)
project(code LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts SerialPort)

qt_standard_project_setup()

qt_add_executable(code
    WIN32 MACOSX_BUNDLE
    main.cpp
    janelaprincipal.cpp
    janelaprincipal.h
    graficoplotter.h graficoplotter.cpp
    leitorserial.h leitorserial.cpp
    painelcontrole.h painelcontrole.cpp
    gerenciadordados.h gerenciadordados.cpp
)

target_link_libraries(code
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Charts
        Qt::SerialPort


)

install(TARGETS code
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#qt_generate_deploy_app_script(
#    TARGET code
 #   OUTPUT_SCRIPT deploy_script
 #   NO_UNSUPPORTED_PLATFORM_ERROR
#)
#install(SCRIPT ${deploy_script})
