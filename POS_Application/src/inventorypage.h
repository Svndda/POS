#ifndef INVENTORYPAGE_H
#define INVENTORYPAGE_H

#include <QWidget>
#include <map>
#include <vector>
#include <string>
#include "appmodel.h"
#include "product.h"
#include "supplyitem.h"

namespace Ui {
class InventoryPage;
}

class InventoryPage : public QWidget {
  Q_OBJECT

public:
  explicit InventoryPage(QWidget *parent = nullptr,
      AppModel& model = AppModel::getInstance());
  ~InventoryPage();
  
  // Private class attributes.
private:
  Ui::InventoryPage *ui;
  // Reference to the application model.
  AppModel& appModel;
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

#endif // INVENTORYPAGE_H
