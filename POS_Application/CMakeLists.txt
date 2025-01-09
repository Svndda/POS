cmake_minimum_required(VERSION 3.19)
project(POS_Application LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(POS_Application
  WIN32 MACOSX_BUNDLE
  src/main.cpp
  src/controller/appcontroller.cpp src/controller/appcontroller.h
  src/controller/mainwindow.ui
  
  src/ui/loginpage.h src/ui/loginpage.cpp src/ui/loginpage.ui
  src/ui/inventorypage.h src/ui/inventorypage.cpp src/ui/inventorypage.ui
  
  src/model/backupmodule.h src/model/backupmodule.cpp
  src/model/posmodel.h src/model/posmodel.cpp
  src/model/product.h src/model/product.cpp
  src/model/supplyitem.h
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

# Adds the target directories.
target_include_directories(POS_Application
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/controller
        ${CMAKE_SOURCE_DIR}/src/ui
        ${CMAKE_SOURCE_DIR}/src/model
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
