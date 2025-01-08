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
  
  // Private class attributes.
private:
  Ui::InventoryPage *ui;
  // Object private data structures that contains the POS registered drinks.
  std::map<std::string, std::vector<Product>> registeredDrinks;
  // Object private data structures that contains the POS registered dishes.
  std::map<std::string, std::vector<Product>> registeredDishes;
  // Object private data structures that contains the POS registered products
  //  combined.
  std::vector<std::pair<std::string, Product>> registeredProducts;
  size_t productPageIndex = 0;
  
  // Private class functions.
private:
  void registerProductType(
      std::vector<std::pair<std::string, Product>>& registeredProducts
      , const std::map<std::string, std::vector<Product>>& productTypeRegister);
  QString formatProductIngredients(const std::vector<SupplyItem>& ingredients);
  void updateProductsInformation(
      std::vector<std::pair<std::string, Product>> visibleProducts);
  void updateProductLabel(size_t labelIt,
      const QString& labelSuffix, const QString& value);
  
private slots:
  void on_nextProductPage_button_clicked();
  void on_previousProductPage_button_clicked();
};

#endif // INVENTORY_H
