cmake_minimum_required(VERSION 3.19)
project(POS_Application LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(POS_Application
  WIN32 MACOSX_BUNDLE
  src/main.cpp
  src/appcontroller.cpp src/appcontroller.h
  src/mainwindow.ui
  src/loginpage.h src/loginpage.cpp src/loginpage.ui
  src/inventorypage.h src/inventorypage.cpp src/inventorypage.ui
  
  src/backupmodule.h src/backupmodule.cpp
  src/appmodel.h src/appmodel.cpp
  src/product.h src/product.cpp
  src/supplyitem.h
)

qt_add_translations(
    TARGETS POS_Application
    TS_FILES POS_Application_es_CR.ts
)

target_link_libraries(POS_Application
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS POS_Application
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET POS_Application
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
