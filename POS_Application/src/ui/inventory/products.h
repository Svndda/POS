#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <QWidget>
#include <map>
#include <vector>
#include <string>
#include "posmodel.h"
#include "product.h"
#include "inventory.h"

namespace Ui {
class Products;
}

class Products : public Inventory {
  Q_OBJECT
  
private:
  Ui::Products* ui;

public:
  explicit Products(QWidget *parent = nullptr,
      POS_Model& model = POS_Model::getInstance());
  ~Products();
  
protected:
  void refreshDisplay(const size_t pageItems) override;
  void setupConnections() override;
  
  // Private class functions.
private:
  void registerProductType(
      std::vector<std::pair<std::string, Product>>& registeredProducts
      , const std::map<std::string, std::vector<Product>>& productTypeRegister);
  void refreshProductDisplay(
      std::vector<std::pair<std::string, Product>> visibleProducts
      , const size_t items);
  void updateProductLabel(size_t labelIt,
      const QString& labelSuffix, const QString& value);
  void deleteRegisteredProduct(size_t index);
  void editProductInformation(size_t index);
  
private slots:
  void addProduct_button_clicked();
  
protected slots:
  virtual void on_nextPage_button_clicked() override;
  virtual void on_previousPage_button_clicked() override;
  virtual void on_delete_button_clicked() override;
  virtual void on_edit_button_clicked() override;
  virtual void on_categories_button_clicked() override;
  virtual void on_supplies_button_clicked() override;
  virtual void on_products_button_clicked() override;
};

#endif // Products_H
