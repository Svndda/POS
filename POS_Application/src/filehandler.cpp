#include <algorithm>
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include "filehandler.h"
#include "supplyitem.h"

FileHandler::FileHandler(const std::vector<std::string>& backupsVector) :
    filenames(backupsVector) {
}

FileHandler& FileHandler::getInstance(const std::vector<std::string>&
      backupsVector) {
  // Creates an static instance of the class to avoid duplication.
  static FileHandler instance(backupsVector);
  return instance;
}

std::map<std::string, std::vector<Product>> FileHandler::readDrinksBackup() {
  return this->readProductsBackup(this->filenames[0]);
}

std::map<std::string, std::vector<Product>> FileHandler::readDishesBackup() {
  return this->readProductsBackup(this->filenames[1]);
}

std::map<std::string, std::vector<Product>> FileHandler::readProductsBackup(
    const std::string& filename) {
  
  std::ifstream file(filename);
  if (!file) {
    throw std::runtime_error("No se pudo abrir el archivo: " + filename);
  }
  
  std::map<std::string, std::vector<Product>> registeredProducts;
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
    }
    // identify the product's name.
    else if (line.back() == '-') {
      productName = line.substr(0, line.size() - 1);
    }
    // Reads the product ingredients and price.
    else {
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
          // Separates the ingredient name and quantity.
          std::string ingredientName = productInfo.substr(0, separatorPos);
          std::string quantityStr = productInfo.substr(separatorPos + 1);
          try {
            // Extracts the ingredient quantity and creates an supplyItem.
            uint64_t ingredientQuantity = std::stoull(quantityStr);
            productIngredients.emplace_back(ingredientName, ingredientQuantity);
          } catch (const std::exception&) {
            throw std::runtime_error("Error al analizar: " + productInfo);
          }
        } else {
          // If theres no spacer, then its the product's price.
          try {
            productPrice = std::stoull(productInfo);
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
  // Returns all the registered products of the given backup.
  return registeredProducts;
}

void FileHandler::writeDrinksBackUp(
    const std::map<std::string, std::vector<Product>>& registeredDrinks) {
  this->writeProductsBackup(this->filenames[0], registeredDrinks);
}

void FileHandler::writeDishesBackUp(
    const std::map<std::string, std::vector<Product>>& registeredDishes) {
  this->writeProductsBackup(this->filenames[1], registeredDishes);
}

void FileHandler::writeProductsBackup(
    const std::string& filename,
    const std::map<std::string, std::vector<Product>>& registeredProducts) {
  
  // Open the file in write mode and erase the content.
  std::ofstream file(filename);
  if (!file) {
    throw std::runtime_error("No se pudo abrir el archivo para escritura: " + filename);
  }
  
  // Transverse the map through all the product categories.
  for (const auto& [category, products] : registeredProducts) {
    // Writes out the actual category for the next products.
    file << category << ":" << std::endl;
    
    // Transverse and write out all the products in the actual category.
    for (const auto& product : products) {
      // Writes out the product's name, followed of thr product name identifier.
      // '-'.
      file << product.getName() << " -" << std::endl;
      
      // Writes out the products ingredients and their quantity.
      for (const auto& ingredient : product.getIngredients()) {
        file << ingredient.getName() << " ; "
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


