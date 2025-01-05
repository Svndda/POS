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
  BackUpController();
  void start();
};

#endif // BACKUPCONTROLLER_H
