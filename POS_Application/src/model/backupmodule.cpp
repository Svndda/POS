#include <QString>
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include "backupmodule.h"
#include "supplyitem.h"


BackupModule::BackupModule() {
}

BackupModule& BackupModule::getInstance() {
  // Creates an static instance of the class to avoid duplication.
  static BackupModule instance;
  return instance;
}

std::map<std::string, std::vector<Product>> BackupModule::getProductsBackup() {
  std::map<std::string, std::vector<Product>> registeredProducts;
  this->readProductsBackup(this->PRODUCTS_BACKUP_FILE, registeredProducts);
  return registeredProducts;
}

std::vector<SupplyItem> BackupModule::getSuppliesBackup() {
  std::vector<SupplyItem> registeredSupplies;
  this->readSupplyItemsBackup(registeredSupplies);
  return registeredSupplies;
}

void BackupModule::readProductsBackup(
    const std::string& filename
    , std::map<std::string, std::vector<Product>>& registeredProducts) {
  
  std::ifstream file(filename);
  if (!file) {
    throw std::runtime_error("No se pudo abrir el archivo: " + filename);
  }
  
  std::string line;
  std::string productCategory;
  std::string productName;
  
  while (std::getline(file, line)) {
    // Ignores the empty lines.
    if (line.empty()) continue;
    
    // Identify the product categories.
    if (line.back() == ':') {
      productCategory = line.substr(0, line.size() - 1);
      // Initialize the category vector.
      registeredProducts[productCategory] = {};
    // identify the product's name.
    } else if (line.back() == '-') {
      std::cout << line << " producto" << std::endl;
      QString name(line.substr(0, line.size() - 1).data());
      productName = name.trimmed().toStdString();
    // Reads the product ingredients and price.
    } else {
      std::istringstream stream(line);
      std::vector<SupplyItem> productIngredients;
      std::string productInfo;
      uint64_t productPrice = 0;
      
      while (std::getline(stream, productInfo, '\t')) {
        // Normalize the tabs in the strings.
        productInfo.erase(0, productInfo.find_first_not_of(" \t"));
        productInfo.erase(productInfo.find_last_not_of(" \t") + 1);
        
        // Finds the ingredient's separator.
        size_t separatorPos = productInfo.find(';');
        if (separatorPos != std::string::npos) {
          // Separates the ingredient's name and quantity.
          QString ingredientName(productInfo.substr(0, separatorPos).data());
          ingredientName = ingredientName.trimmed();
          
          std::string quantityStr = productInfo.substr(separatorPos + 1);
          try {
            // Extracts the ingredient quantity and creates an supplyItem.
            uint64_t ingredientQuantity = std::stoull(quantityStr);
            productIngredients.emplace_back(ingredientName.toStdString()
                , ingredientQuantity);
          } catch (const std::exception&) {
            throw std::runtime_error("Error al analizar: " + productInfo);
          }
        } else {
          // If theres no spacer, then its the product's price.
          try {
            productPrice = std::stod(productInfo);
          } catch (const std::exception&) {
            throw std::runtime_error("Error al convertir el precio: " + productInfo);
          }
        }
      }
      
      // Creates a new product for the corresponding category.
      registeredProducts[productCategory].emplace_back(1, productName,
          productIngredients, productPrice);
    }
  }
  file.close();
}

void BackupModule::readSupplyItemsBackup(std::vector<SupplyItem>& supplies) {
  std::ifstream file(this->SUPPLIES_BACKUP_FILE);
  if(!file) {
    throw std::runtime_error("No se pudo abrir el archivo para escritura: "
        + this->SUPPLIES_BACKUP_FILE);
  }
  
  std::string line;
  std::string name;;
  uint64_t quantity;
  
  size_t index = 0;
  while(std::getline(file, line)) {
    std::istringstream stream(line);
    stream >> name >> quantity;
    supplies.emplace_back(name, quantity);
    std::cout << supplies[index] << std::endl;
    ++index;
  }
}

void BackupModule::writeRegistersBackUp(
    const std::map<std::string, std::vector<Product>>& products
    , const std::vector<SupplyItem>& supplies) {
  this->writeProductsBackup(this->PRODUCTS_BACKUP_FILE, products);
  this->writeSuppliesBackup(supplies);
}

void BackupModule::writeProductsBackup(
    const std::string& filename,
    const std::map<std::string, std::vector<Product>>& registeredProducts) {
  
  // Open the file in write mode and erase the content.
  std::ofstream file(filename);
  if (!file) {
    throw std::runtime_error("No se pudo abrir el archivo para escritura: " + filename);
  }
  
  // Transverse the map through all the product categories.
  for (const auto& [category, products] : registeredProducts) {
    QString productCategtory(category.data());
    productCategtory = productCategtory.trimmed();
    // Writes out the actual category for the next products.
    file << productCategtory.toStdString() << ":" << std::endl;
    
    // Transverse and write out all the products in the actual category.
    for (const auto& product : products) {
      // Writes out the product's name, followed of thr product name identifier.
      // '-'.
      file << product.getName() << " -" << std::endl;
      
      // Writes out the products ingredients and their quantity.
      for (const auto& ingredient : product.getIngredients()) {
        QString ingredientName(ingredient.getName().data());
        ingredientName = ingredientName.trimmed();
        file << ingredientName.toStdString() << " ; "
            << ingredient.getQuantity() << "\t";
      }
      
      // Writes out the product's price as the last character of the line.
      file << product.getPrice() << std::endl;
    }
    
    // Writes out a blank line between categories.
    file << std::endl;
  }
  
  file.close();
}

void BackupModule::writeSuppliesBackup(const std::vector<SupplyItem>& supplies) {
  std::ofstream file(this->SUPPLIES_BACKUP_FILE);
  if (!file) {
    throw std::runtime_error("No se pudo abrir el archivo para escritura: "
        + this->SUPPLIES_BACKUP_FILE);
  }
  
  for (const auto& supply : supplies) {
    file << supply << "\n";
  }
  
  file.close();
}
