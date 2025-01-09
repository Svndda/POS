#ifndef APPMODEL_H
#define APPMODEL_H

#include <vector>
#include <map>
#include <string>
#include "backupmodule.h"
#include "product.h"

class AppModel {
private:
  BackupModule& backupModule;
  std::map<std::string, std::vector<Product>> Dishes;
  std::map<std::string, std::vector<Product>> Drinks;
  std::vector<SupplyItem> itemsOnInventory;
  bool started = false;
  
  // Private class function.
private:
  AppModel(BackupModule& module);
  
  // Public class function.
public:
  static AppModel& getInstance();
  
  void start();
  
  void shutdown();
  
  inline bool isStarted() {
    return this->started;  
  }
  
  inline std::map<std::string, std::vector<Product>> getRegisteredDrinks() {
    return this->Drinks;
  }
  
  inline std::map<std::string, std::vector<Product>> getRegisteredDishes() {
    return this->Dishes;
  }
};

#endif // APPMODEL_H
