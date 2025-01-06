#include <string>
#include <vector>
#include <map>
#include <utility>
#include "inventory.h"
#include "ui_inventory.h"
#include "backupcontroller.h"
#include "product.h"

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
  
  Product product = visibleProducts[1].second;
  QString productCategory = visibleProducts[1].first.data();
  this->ui->productName_label_0->setText(product.getName().data());
  this->ui->productCategory_label_0->setText(productCategory);
}

Inventory::~Inventory() {
  delete ui;
}
