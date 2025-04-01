cmake_minimum_required(VERSION 3.19)
project(POS_Application LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS
  Core Widgets
  LinguistTools
  PrintSupport
)

qt_standard_project_setup()

qt_add_executable(POS_Application
  WIN32 MACOSX_BUNDLE
  src/main.cpp
  
  src/model/backupmodule.h src/model/backupmodule.cpp
  src/model/model.h src/model/model.cpp
  src/model/product.h src/model/product.cpp
  src/model/supply.h src/model/supply.cpp
  src/model/receipt.h src/model/receipt.cpp
  src/model/user.h src/model/user.cpp
  
  src/common/util.cpp src/common/util.h
  
  src/controller/appcontroller.cpp
  src/controller/appcontroller.h
  src/controller/mainwindow.ui
  
  src/ui/loginpage.h
  src/ui/loginpage.cpp
  src/ui/loginpage.ui
  
  src/ui/pos/pos.h src/ui/pos/pos.cpp
  src/ui/pos/pos.ui
  
  src/ui/pos/billing/billingpage.h
  src/ui/pos/billing/billingpage.cpp
  src/ui/pos/billing/billingpage.ui
  src/ui/pos/billing/orderselectionbutton.h
  src/ui/pos/billing/orderselectionbutton.cpp
  src/ui/pos/billing/orderselectionbutton.ui
  src/ui/pos/billing/productselectionbutton.h
  src/ui/pos/billing/productselectionbutton.cpp
  src/ui/pos/billing/productselectionbutton.ui
  src/ui/pos/billing/order/order.h
  src/ui/pos/billing/order/order.cpp
  src/ui/pos/billing/order/order.ui
  src/ui/pos/billing/order/orderelement.h
  src/ui/pos/billing/order/orderelement.cpp
  src/ui/pos/billing/order/orderelement.ui
  src/ui/pos/billing/dialogs/processorderdialog.h
  src/ui/pos/billing/dialogs/processorderdialog.cpp
  src/ui/pos/billing/dialogs/processorderdialog.ui
  
  src/ui/pos/cashier/cashierpage.h
  src/ui/pos/cashier/cashierpage.cpp
  src/ui/pos/cashier/cashierpage.ui
  src/ui/pos/cashier/expenselabel.h
  src/ui/pos/cashier/expenselabel.cpp
  src/ui/pos/cashier/expenselabel.ui
  src/ui/pos/cashier/incomelabel.h
  src/ui/pos/cashier/incomelabel.cpp
  src/ui/pos/cashier/incomelabel.ui
  src/ui/pos/cashier/dialogs/expensedialog.h
  src/ui/pos/cashier/dialogs/expensedialog.cpp
  src/ui/pos/cashier/dialogs/expensedialog.ui
  src/ui/pos/cashier/dialogs/cashierdialog.h
  src/ui/pos/cashier/dialogs/cashierdialog.cpp
  src/ui/pos/cashier/dialogs/cashierdialog.ui
  
  src/ui/inventory/inventory.h
  src/ui/inventory/inventory.cpp
  src/ui/inventory/inventory.ui
  src/ui/inventory/catalog.h
  
  src/ui/inventory/products/productscatalog.h
  src/ui/inventory/products/productscatalog.cpp
  src/ui/inventory/products/productscatalog.ui
  src/ui/inventory/products/dialogs/productformdialog.h
  src/ui/inventory/products/dialogs/productformdialog.cpp
  src/ui/inventory/products/dialogs/productformdialog.ui
  
  src/ui/inventory/categories/categoriescatalog.h
  src/ui/inventory/categories/categoriescatalog.cpp
  src/ui/inventory/categories/categoriescatalog.ui
  src/ui/inventory/categories/dialogs/categoryformdialog.h
  src/ui/inventory/categories/dialogs/categoryformdialog.cpp
  src/ui/inventory/categories/dialogs/categoryformdialog.ui
  
  src/ui/inventory/supplies/supplyformdialog.h
  src/ui/inventory/supplies/supplyformdialog.cpp
  src/ui/inventory/supplies/supplyformdialog.ui
  src/ui/inventory/supplies/dialogs/suppliescatalog.h
  src/ui/inventory/supplies/dialogs/suppliescatalog.cpp
  src/ui/inventory/supplies/dialogs/suppliescatalog.ui

  src/ui/users/users.h
  src/ui/users/users.cpp
  src/ui/users/users.ui
  src/ui/users/dialogs/userformdialog.h
  src/ui/users/dialogs/userformdialog.cpp
  src/ui/users/dialogs/userformdialog.ui
  
  src/ui/settings/settings.h
  src/ui/settings/settings.cpp
  src/ui/settings/settings.ui
  
  src/ui/settings/personalization.h
  src/ui/settings/personalization.cpp
  src/ui/settings/personalization.ui
  
  src/ui/settings/logout.h
  src/ui/settings/logout.cpp
  src/ui/settings/logout.ui
  
  src/ui/settings/assistance.h
  src/ui/settings/assistance.cpp
  src/ui/settings/assistance.ui
)

qt_add_translations(
    TARGETS POS_Application
    TS_FILES POS_Application_es_CR.ts
)

target_link_libraries(POS_Application
    PRIVATE
        Qt6::Core
        Qt::Widgets
        Qt::PrintSupport
)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

# Adds the target directories.
target_include_directories(POS_Application
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/common        
        ${CMAKE_SOURCE_DIR}/src/model
        ${CMAKE_SOURCE_DIR}/src/controller
        ${CMAKE_SOURCE_DIR}/src/ui
        ${CMAKE_SOURCE_DIR}/src/ui/pos/
        ${CMAKE_SOURCE_DIR}/src/ui/pos/billing
        ${CMAKE_SOURCE_DIR}/src/ui/pos/billing/order
        ${CMAKE_SOURCE_DIR}/src/ui/pos/billing/dialogs
        ${CMAKE_SOURCE_DIR}/src/ui/pos/cashier
        ${CMAKE_SOURCE_DIR}/src/ui/pos/cashier/dialogs
        ${CMAKE_SOURCE_DIR}/src/ui/inventory
        ${CMAKE_SOURCE_DIR}/src/ui/inventory/products
        ${CMAKE_SOURCE_DIR}/src/ui/inventory/products/dialogs
        ${CMAKE_SOURCE_DIR}/src/ui/inventory/categories
        ${CMAKE_SOURCE_DIR}/src/ui/inventory/categories/dialogs
        ${CMAKE_SOURCE_DIR}/src/ui/inventory/supplies
        ${CMAKE_SOURCE_DIR}/src/ui/inventory/supplies/dialogs
        ${CMAKE_SOURCE_DIR}/src/ui/users
        ${CMAKE_SOURCE_DIR}/src/ui/users/dialogs
        ${CMAKE_SOURCE_DIR}/src/ui/settings
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
