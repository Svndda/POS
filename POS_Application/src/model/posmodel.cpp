#include <vector>
#include <QDebug>

#include "posmodel.h"
#include "backupmodule.h"

#define DRINKS_BACKUP_FILE "C:\\Users\\aaron\\Dev\\Repositories" \
"\\POS\\POS_Application\\backup\\products\\Bebidas.txt"

#define DISHES_BACKUP_FILE "C:\\Users\\aaron\\Dev\\Repositories" \
"\\POS\\POS_Application\\backup\\products\\Comidas.txt"

#define PRIME_BACKUP_FILE ""

POS_Model::POS_Model(BackupModule& module)
    : backupModule(module) {
}

POS_Model& POS_Model::getInstance() {
  // Loads the data contained in the backup program files.
  std::vector<std::string> backUpFiles = {
      DRINKS_BACKUP_FILE,
      DISHES_BACKUP_FILE,
      ""};
  static POS_Model instance(BackupModule::getInstance(backUpFiles));
  return instance;
}

void POS_Model::start() {
  // Reads and store the backup to the program memory to use them in the
  // program execution.
  this->Drinks = this->backupModule.readDrinksBackup();
  this->Dishes = this->backupModule.readDishesBackup();
  
  // Changes the started value to true.
  this->started = true;
}

std::vector<std::pair<std::string, Product>> POS_Model::registeredProducts() {
  std::vector<std::pair<std::string, Product>> mergedProducts;
  this->extractProducts(mergedProducts, this->Drinks);
  this->extractProducts(mergedProducts, this->Dishes);
  // qDebug() << "products vector size: " << mergedProducts.size();
  return mergedProducts;
};

void POS_Model::updateProductRegisters(const std::string productCategory,
    const Product& product) {
  
  if (this->insertOnRegister(productCategory, product, this->Drinks)) {
    qDebug() << "producto nuevo de bebidas";
    this->backupModule.writeDrinksBackUp(this->Drinks);
  } else if (this->insertOnRegister(productCategory, product, this->Dishes)) {
    qDebug() << "producto nuevo de comidas";
    this->backupModule.writeDishesBackUp(this->Dishes);
  }
}

bool POS_Model::insertOnRegister(const std::string productCategory
    , const Product& product
    , std::map<std::string, std::vector<Product>>& productTypeRegister) {
  for (auto& category : productTypeRegister) {
    if (category.first == productCategory) {
      qDebug() << "Categoria coincidente: " << category.first;
      category.second.emplace_back(product);
      return true;
    }
  }
  return false;
}

void POS_Model::extractProducts(
    std::vector<std::pair<std::string, Product>>& registeredProducts
    , const std::map<std::string, std::vector<Product>>& productTypeRegister) {
  // Transverse all the product type categories and their products.
  for (const auto& [category, products] : productTypeRegister) {
    // Transverse all the products of the category and added them to the vector.
    for (const auto& product : products) {
      // Emplace the product in the the product register.
      registeredProducts.emplace_back(category, product);
    }
  }
}

void POS_Model::shutdown() {
  // Writes out the registers of the dishes and drinks back to the backup files.
  this->backupModule.writeDishesBackUp(this->Dishes);
  this->backupModule.writeDrinksBackUp(this->Drinks);
  // Clears the vector memory.
  this->Dishes.clear();
  this->Drinks.clear();
  this->started = false;
}
