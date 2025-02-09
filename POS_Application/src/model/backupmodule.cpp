// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include <QString>
#include <map>
#include <fstream>
#include <iostream>
#include <qdebug.h>
#include <qdir.h>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include <filesystem>

#include "backupmodule.h"
#include "user.h"
#include "supply.h"

BackupModule::BackupModule() {
}

BackupModule& BackupModule::getInstance() {
  // Creates an static instance of the class to avoid duplication.
  static BackupModule instance;
  return instance;
}

std::map<std::string, std::vector<Product>> BackupModule::getProductsBackup() {
  // Temporal map to store the products by a category key.
  std::map<std::string, std::vector<Product>> categoryRegisters;
  // Reads the information contained in the products backup file and store it
  // in the map.
  this->readProductsBackup(this->PRODUCTS_BACKUP_FILE, categoryRegisters);
  return categoryRegisters;
}

std::vector<Supply> BackupModule::getSuppliesBackup() {
  // Temporal vector to store the registered supplies.
  std::vector<Supply> registeredSupplies;
  // Reads the supplies information contained in the supplies backup file.
  this->readSupplyItemsBackup(registeredSupplies);
  return registeredSupplies;
}

std::vector<User> BackupModule::getUsersBackup() {
  // Temporal vector to store the registered users's information.
  std::vector<User> registeredUsers;
  // Obtains and store the users information into the temporal vector.
  this->readUsersBackup(registeredUsers);
  // Returns the users's information.
  return registeredUsers;
}

void BackupModule::readProductsBackup(
    const std::string& filename
    , std::map<std::string, std::vector<Product>>& registeredProducts) {
  
  // Try to open the input file name/file path.
  std::ifstream file(filename);
  if (!file) {
    throw std::runtime_error("No se pudo abrir el archivo: " + filename);
  }
  
  // Crear un objeto de tipo std::filesystem::path
  std::filesystem::path filePath(PRODUCTS_BACKUP_FILE);
  
  // Obtener el padre del archivo (directorio que lo contiene)
  std::filesystem::path parentPath = filePath.parent_path();
  
  // Temporal variable to store a line of characters of the file.
  std::string line;
  // Temporal variables to store the category of the product and his name.
  std::string productCategory;
  std::string productName;
  std::string imagePath;
  
  // While there's a line on the input file.
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
      // Parse the product name and trim it to avoid blank spaces.
      QString name(line.substr(0, line.size() - 1).data());
      // Sets the product name.
      productName = name.trimmed().toStdString();
    // Reads the product ingredients and price.
    } else {
      // Creates a stream from the line.
      std::istringstream stream(line);
      // Temporal string that will be parsing the product information.
      std::string productInfo;
      // Temporal vector to store the product's supplies.
      std::vector<Supply> productIngredients;
      uint64_t productPrice = 0;
      
      // While there's tabs of the actual line/stream
      while (std::getline(stream, productInfo, '\t')) {
        // Normalize the tabs in the strings.
        productInfo.erase(0, productInfo.find_first_not_of(" \t"));
        productInfo.erase(productInfo.find_last_not_of(" \t") + 1);
        
        // Finds the ingredient's separator.
        size_t separatorPos = productInfo.find(';');
        if (separatorPos != std::string::npos) {
          // Separates the ingredient's name and quantity.
          QString ingredientName(productInfo.substr(0, separatorPos).data());
          // Obtain the name of the product's ingredient.
          ingredientName = ingredientName.trimmed();
          // Obtain the quantity of the product's ingredient.
          std::string quantityStr = productInfo.substr(separatorPos + 1);
          try {
            // Extracts the ingredient quantity and creates an supplyItem.
            uint64_t ingredientQuantity = std::stoull(quantityStr);
            // Emplace a new ingredient on the product's ingredients vector.
            productIngredients.emplace_back(ingredientName.toStdString()
                , ingredientQuantity);
          } catch (const std::exception&) {
            throw std::runtime_error("Error al analizar: " + productInfo);
          }
        } else {
          // If theres no spacer, then its the product's price.
          try {
            productPrice = std::stod(productInfo);
          } catch (const std::exception& e) {
            imagePath = productInfo;
            // Erase the blanck spaces on the image path.
            imagePath.erase(
                std::remove_if(imagePath.begin(), imagePath.end(), isspace),
                imagePath.end());
            // Construir la ruta completa usando std::filesystem::path
            std::filesystem::path fullPath = parentPath / imagePath;
            imagePath = fullPath.string();
          }
        }
      }
      
      // Creates a new product for the corresponding category.
      registeredProducts[productCategory].emplace_back(1, productName,
        productIngredients, productPrice, QPixmap(imagePath.c_str()));
      
      qDebug() << imagePath << " cargando imagen de";      
    }
  }
  file.close();
}

void BackupModule::readSupplyItemsBackup(std::vector<Supply>& supplies) {
  std::ifstream file(this->SUPPLIES_BACKUP_FILE);
  // Try to open the input file name/file path.  
  if(!file) {
    throw std::runtime_error("No se pudo abrir el archivo para lectura: "
        + this->SUPPLIES_BACKUP_FILE);
  }
  
  // Temporal to store the indexed line of the supply's file.
  std::string line;
  // Temporals to store the supply's information.
  std::string name;;
  uint64_t quantity;
  std::string measure;
  
  // While there's line to read on the file.
  while(std::getline(file, line)) {
    // Creates a buffer from the actual line.
    std::istringstream stream(line);
    // Reads the name, quantity and measure of the supply backup.
    stream >> name >> quantity >> measure;
    // Emplace a new supply on the givel vector of supplies.
    supplies.emplace_back(name, quantity, measure);
  }
}

void BackupModule::readUsersBackup(std::vector<User>& registeredUsers) {
  // Opens the users's backup file in binary read mode.
  std::ifstream inFile(this->USERS_BACKUP_FILE, std::ios::binary);
  if (!inFile) {
    throw std::runtime_error("No se pudo abrir el archivo para lectura: "
        + this->SUPPLIES_BACKUP_FILE); 
  }
  
  // Reads the number of registered users.
  size_t numUsers = -1;
  inFile.read(reinterpret_cast<char*>(&numUsers), sizeof(numUsers));
  // Cleans the given vector before hand.
  registeredUsers.clear();
  qDebug() << "usuarios a leer: " << numUsers;
  // Reads the user's information.
  for (size_t i = 0; i < numUsers; ++i) {
    // Creates a user data object.
    User user;
    // Reads an user information the backup file.
    user.loadFromBinary(inFile);
    // Add the user data into the program memory.
    registeredUsers.push_back(user);
  }
  // Close the backup file.
  inFile.close();
}

void BackupModule::updateProductsBackup(
    const std::map<std::string, std::vector<Product>>& products) {
  // Writes out the given products information into the product's backup files.
  this->writeProductsBackup(this->PRODUCTS_BACKUP_FILE, products);
}

void BackupModule::updateSuppliesBackup(
    const std::vector<Supply>& supplies) {
  // Writes out the given supplies information into the supplies's backup files.
  this->writeSuppliesBackup(supplies);
}

void BackupModule::updateUsersBackup(const std::vector<User>& users) {
  // Writes out the given users information into the users's backup files.
  this->writeUsersBackup(users);
} 

void BackupModule::writeUsersBackup(const std::vector<User>& users) {
  // Opens the file in binary write mode.
  std::ofstream outFile(this->USERS_BACKUP_FILE, std::ios::binary);
  if (!outFile) {
    std::cerr << "Error: No se pudo abrir el archivo para escritura.\n";
    return;
  }
  
  // Save the users quantity into the file.
  size_t numUsers = users.size();
  outFile.write(reinterpret_cast<const char*>(&numUsers), sizeof(numUsers));
  
  // Writes out the user information binary into the given file.
  for (const auto& user : users) {
    user.saveToBinary(outFile);
  }
  
  outFile.close();
}

void BackupModule::writeProductsBackup(
    const std::string& filename,
    const std::map<std::string, std::vector<Product>>& registeredProducts) {
  
  // Open the file in write mode and erase the content.
  std::ofstream file(filename);
  if (!file) {
    throw std::runtime_error("No se pudo abrir el archivo para escritura: "
        + filename);
  }
  
  // Creates a std::filesystem::path object.
  std::filesystem::path filePath(filename);
  
  // Obtains the parent directory's name.
  std::filesystem::path parentPath = filePath.parent_path();
  
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
      // Creates the string name of the product's image backup file.
      std::string imageName = product.getName() +  ".png";
      // Erase the blanck spaces on the image path.
      imageName.erase(
          std::remove_if(imageName.begin(), imageName.end(), isspace),
          imageName.end());
      // Writes out the product's price as the last character of the line.
      file << product.getPrice() << "\t" << imageName << std::endl;
      
      // Saves the product image in the save directory as the backup file.
      const QPixmap& productImage = product.getImage();
      const std::string directory = parentPath.string() + "\\" + imageName;
      
      // Checks if the direcoty already exist, if not, creates it.
      QDir dir(QString::fromStdString(parentPath.string()));
      if (!dir.exists()) {
        dir.mkpath(".");
      }
      
      try {
        productImage.save(QString::fromStdString(directory));
        qDebug() << directory << " salvango imagen en";
      } catch(std::runtime_error& e) {
        qDebug() << "El producto: " << product.getImage() << ". No tiene imagen";
      }
    }
    
    // Writes out a blank line between categories.
    file << std::endl;
  }
  
  file.close();
}

void BackupModule::writeSuppliesBackup(
    const std::vector<Supply>& supplies) {
  // Try to open the specified file of the supplies backup.
  std::ofstream file(this->SUPPLIES_BACKUP_FILE);
  if (!file) {
    throw std::runtime_error("No se pudo abrir el archivo para escritura: "
        + this->SUPPLIES_BACKUP_FILE);
  }
  
  // Writes out the supplies information into the file.
  for (const auto& supply : supplies) {
    file << supply << "\n";
  }
  
  file.close();
}
