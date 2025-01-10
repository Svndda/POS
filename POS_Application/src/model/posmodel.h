#ifndef POSMODEL_H
#define POSMODEL_H

#include <vector>
#include <map>
#include <string>
#include "backupmodule.h"
#include "product.h"

class POS_Model {
private:
  BackupModule& backupModule;
  std::map<std::string, std::vector<Product>> Dishes;
  std::map<std::string, std::vector<Product>> Drinks;
  std::vector<SupplyItem> itemsOnInventory;
  bool started = false;
  
  // Private class function.
private:
  POS_Model(BackupModule& module);
  
  // Public class function.
public:
  static POS_Model& getInstance();
  
  void start();
  
  void shutdown();
  
  std::vector<std::pair<std::string, Product>> registeredProducts();
  
  void updateProductRegisters(const std::string productCategory,
                              const Product& product);
  
  // CLass getters.
public:
  inline bool isStarted() {
    return this->started;  
  }
  
  inline std::map<std::string, std::vector<Product>> getRegisteredDrinks() {
    return this->Drinks;
  }
  
  inline std::map<std::string, std::vector<Product>> getRegisteredDishes() {
    return this->Dishes;
  }
  
private:
  void extractProducts(
      std::vector<std::pair<std::string, Product>>& registeredProducts
      , const std::map<std::string, std::vector<Product>>& productTypeRegister);
  bool insertOnRegister(const std::string productCategory
      , const Product& product
      , std::map<std::string, std::vector<Product>>& productTypeRegister);
};

#endif // APPMODEL_H
