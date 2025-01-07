#include <string>
#include <vector>
#include <map>
#include <utility>
#include "inventory.h"
#include "ui_inventory.h"
#include "backupcontroller.h"
#include "product.h"

QLabel* cloneLabel(QLabel* original) {
  QLabel* newLabel = new QLabel(original->parentWidget());
  
  // Copiar texto y propiedades importantes
  newLabel->setText(original->text());
  newLabel->setAlignment(original->alignment());
  newLabel->setStyleSheet(original->styleSheet());
  newLabel->setFont(original->font());
  newLabel->setPixmap(original->pixmap(Qt::ReturnByValue)); // Si usa pixmap
  
  // Otros ajustes si es necesario
  newLabel->setSizePolicy(original->sizePolicy());
  newLabel->setGeometry(original->geometry());
  
  return newLabel;
}

Inventory::Inventory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InventoryPage)
    , registeredDrinks(std::map<std::string, std::vector<Product>>())
    , registeredDishes(std::map<std::string, std::vector<Product>>()){
  ui->setupUi(this);
  BackUpController backupControllerInstance = BackUpController::getInstance();
  backupControllerInstance.start();
  this->registeredDrinks = backupControllerInstance.getRegisteredDrinks();
  this->registeredDishes = backupControllerInstance.getRegisteredDishes();
  
  std::vector<std::pair<std::string, Product>> visibleProducts;
  
  // Iterar sobre las categorías y productos
  for (const auto& [category, products] : this->registeredDishes) {
    // Breaks if theres already 9 product names recolected.
    if (visibleProducts.size() == 9) break;
    for (const auto& product : products) {
      // Breaks if theres already 9 product names recolected.
      if (visibleProducts.size() == 9) break;
      visibleProducts.push_back(std::pair<std::string, Product>(category,
          product));
    }
  }
  std::cout << "Registered Dishes's Category: " << this->registeredDishes.size()
      << std::endl;
  std::cout << "Registered Drink's Category: " << this->registeredDrinks.size()
            << std::endl;
  
  this->updateProductsInformation(visibleProducts);
}

void Inventory::updateProductsInformation(
    std::vector<std::pair<std::string, Product>> visibleProducts) {
  // Tranverse the different rows of products information.
  for (size_t index = 0; index < 9; index++) {
    // Updates the the different labels for each product information that is
    // displayed in the UI.
    this->updateProductsID(visibleProducts, index);
    this->updateProductsName(visibleProducts, index);
    this->updateProductsCategories(visibleProducts, index);
    this->updateProductsPrices(visibleProducts, index);
  }
}

void Inventory::updateProductsID(
    std::vector<std::pair<std::string, Product>> visibleProducts,
    size_t index) {
  // Label row number.
  size_t labelNumber = index;
  // Product's name that are going to be displayed.
  QString productID;
  // Checks if the index is lower than the next visible products to display.
  if (index < visibleProducts.size()) {
    productID = QString(
        std::to_string(visibleProducts[index].second.getID()).data());
  } else {
    // Empty product placeholder
    productID = QString("--------");
  }
  
  // Appends the labelsubfix with the label index.
  QString labelName = QString("productCode_label_%1").arg(labelNumber);
  // Search a label in the ui with that name.
  QLabel* label = this->findChild<QLabel*>(labelName);
  
  // QLabel* newlabel = cloneLabel(label);
  // this->ui->productNameVerticalLayout->addWidget(newlabel);
  
  if (label != nullptr) {
    // Sets the product's name as the label text.
    label->setText(productID.trimmed());
    label->setAlignment(Qt::AlignCenter);
  } else {
    qDebug() << "No se pudo encontrar el puntero del label de ID indicado.";
  }
}

void Inventory::updateProductsName(
    std::vector<std::pair<std::string, Product>> visibleProducts,
    size_t index) {
  // Label row number.
  size_t labelNumber = index;
  // Product's name that are going to be displayed.
  QString productName;
  // Checks if the index is lower than the next visible products to display.
  if (index < visibleProducts.size()) {
    productName = QString(visibleProducts[index].second.getName().data());
  } else {
    // Empty product placeholder
    productName = QString("--------");
  }
  
  // Appends the labelsubfix with the label index.
  QString labelName = QString("productName_label_%1").arg(labelNumber);
  // Search a label in the ui with that name.
  QLabel* label = this->findChild<QLabel*>(labelName);
  
  // QLabel* newlabel = cloneLabel(label);
  // this->ui->productNameVerticalLayout->addWidget(newlabel);
  
  if (label != nullptr) {
    // Sets the product's name as the label text.
    label->setText(productName.trimmed());
    label->setAlignment(Qt::AlignCenter);
  } else {
    qDebug() << "No se pudo encontrar el puntero del label de nombre indicado.";
  }
}

void Inventory::updateProductsCategories(
    std::vector<std::pair<std::string, Product>> visibleProducts,
    size_t index) {
  // Label row number.
  size_t labelNumber = index;
  // Product's category that are going to be displayed.
  QString productCategory;
  if (index < visibleProducts.size()) {
    productCategory = QString((visibleProducts[index].first.data()));
  } else {
    // Empty product placeholder
    productCategory = QString("--------");
  }
  // Appends the labelsubfix with the label index.  
  QString labelName = QString("productCategory_label_%1").arg(labelNumber);
  // Search a label in the ui with that name.  
  QLabel* label = this->findChild<QLabel*>(labelName);
  
  if (label != nullptr) {
    // Sets the product's category as the label text.
    label->setText(productCategory);
  } else {
    qDebug() << "No se pudo encontrar el puntero del label de categoria"
        " indicado.";
  }
}

void Inventory::updateProductsPrices(
    std::vector<std::pair<std::string, Product>> visibleProducts,
    size_t index) {
  // Label row number.
  size_t labelNumber = index;
  // Product's price that are going to be displayed.
  QString productPrice;
  if (index < visibleProducts.size()) {
    productPrice = QString((visibleProducts[index].first.data()));
  } else {
    // Empty product placeholder
    productPrice = QString("--------");
  }
  // Appends the labelsubfix with the label index.  
  QString labelName = QString("productPrice_label_%1").arg(labelNumber);
  // Search a label in the ui with that name.  
  QLabel* label = this->findChild<QLabel*>(labelName);
  
  if (label != nullptr) {
    // Sets the product's price as the label text.
    label->setText(productPrice);
  } else {
    qDebug() << "No se pudo encontrar el puntero del label de precio indicado.";
  }
}

Inventory::~Inventory() {
  delete ui;
}
