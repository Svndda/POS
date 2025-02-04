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
  src/model/supply.h src/model/supply.cpp
  src/common/util.cpp src/common/util.h
  
  src/ui/inventory/inventory.h src/ui/inventory/inventory.cpp
  src/ui/inventory/inventory.ui
  
  src/ui/inventory/productformdialog.h src/ui/inventory/productformdialog.cpp
  src/ui/inventory/productformdialog.ui
  src/controller/mainwindow.ui
  
  
  src/ui/inventory/categoryformdialog.h src/ui/inventory/categoryformdialog.cpp
  src/ui/inventory/categoryformdialog.ui
  src/ui/inventory/suppliescatalog.h src/ui/inventory/suppliescatalog.cpp
  src/ui/inventory/suppliescatalog.ui
  src/ui/inventory/catalog.h
  src/ui/inventory/supplyformdialog.h src/ui/inventory/supplyformdialog.cpp
  src/ui/inventory/supplyformdialog.ui
  src/ui/pos/pos.h src/ui/pos/pos.cpp
  src/ui/pos/pos.ui
  
  
  src/ui/pos/productselectionbutton.h src/ui/pos/productselectionbutton.cpp
  src/ui/pos/productselectionbutton.ui
  src/ui/inventory/categoriescatalog.h src/ui/inventory/categoriescatalog.cpp src/ui/inventory/categoriescatalog.ui
  src/ui/inventory/productscatalog.h src/ui/inventory/productscatalog.cpp src/ui/inventory/productscatalog.ui
  src/ui/pos/receipt.h src/ui/pos/receipt.cpp src/ui/pos/receipt.ui
  src/ui/pos/receiptelement.h src/ui/pos/receiptelement.cpp src/ui/pos/receiptelement.ui
  src/ui/pos/receiptselectionbutton.h src/ui/pos/receiptselectionbutton.cpp src/ui/pos/receiptselectionbutton.ui
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
        ${CMAKE_SOURCE_DIR}/src/ui/pos
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
