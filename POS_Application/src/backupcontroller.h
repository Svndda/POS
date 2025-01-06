#ifndef BACKUPCONTROLLER_H
#define BACKUPCONTROLLER_H

#include <vector>
#include <map>
#include <string>
#include "filehandler.h"
#include "product.h"

class BackUpController {
private:
  std::map<std::string, std::vector<Product>> Dishes;
  std::map<std::string, std::vector<Product>> Drinks;
  std::vector<SupplyItem> itemsOnInventory;
public:
  static BackUpController& getInstance();
  void start();
  inline std::map<std::string, std::vector<Product>> getRegisteredDrinks() {
    return this->Drinks;
  }
  
  inline std::map<std::string, std::vector<Product>> getRegisteredDishes() {
    return this->Dishes;
  }

private:
  BackUpController();
};

#endif // BACKUPCONTROLLER_H
