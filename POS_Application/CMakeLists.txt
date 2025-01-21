cmake_minimum_required(VERSION 3.19)
project(POS_Application LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(POS_Application
  WIN32 MACOSX_BUNDLE
  src/main.cpp
  src/controller/appcontroller.cpp src/controller/appcontroller.h
  
  src/ui/loginpage.h src/ui/loginpage.cpp src/ui/loginpage.ui
  
  src/model/backupmodule.h src/model/backupmodule.cpp
  src/model/posmodel.h src/model/posmodel.cpp
  src/model/product.h src/model/product.cpp
  src/model/supplyitem.h
  src/common/util.cpp src/common/util.h
  
  src/ui/inventory/products.h src/ui/inventory/products.cpp
  src/ui/inventory/products.ui
  
  src/ui/inventory/productformdialog.h src/ui/inventory/productformdialog.cpp
  src/ui/inventory/productformdialog.ui
  src/controller/mainwindow.ui
  src/ui/inventory/categories.h src/ui/inventory/categories.cpp
  src/ui/inventory/categories.ui
  src/ui/inventory/categoryformdialog.h src/ui/inventory/categoryformdialog.cpp
  src/ui/inventory/categoryformdialog.ui
  src/ui/inventory/supplies.h src/ui/inventory/supplies.cpp
  src/ui/inventory/supplies.ui
  src/model/supplyitem.cpp
  src/ui/inventory/inventory.h
  
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
        ${CMAKE_SOURCE_DIR}/src/ui/inventory
        ${CMAKE_SOURCE_DIR}/src/model
        ${CMAKE_SOURCE_DIR}/src/common
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
