#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>
#include <map>
#include <vector>
#include <string>
#include "backupcontroller.h"
#include "product.h"
#include "supplyitem.h"

namespace Ui {
class InventoryPage;
}

class Inventory : public QWidget
{
  Q_OBJECT

public:
  explicit Inventory(QWidget *parent = nullptr);
  ~Inventory();

private:
  Ui::InventoryPage *ui;
  // Object private data structures that contains the POS registered drinks.
  std::map<std::string, std::vector<Product>> registeredDrinks;
  // Object private data structures that contains the POS registered dishes.
  std::map<std::string, std::vector<Product>> registeredDishes;
};

#endif // INVENTORY_H
