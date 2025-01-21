#ifndef CATEGORIES_H
#define CATEGORIES_H

#include "inventory.h"

namespace Ui {
class Categories;
}

class Categories : public Inventory {
  Q_OBJECT
  
private:
  Ui::Categories* ui;
  
public:
  explicit Categories(QWidget *parent = nullptr
      , POS_Model& appModel = POS_Model::getInstance());
  ~Categories();
  
protected:
  void refreshDisplay(const size_t pageItems) override;
  void setupConnections() override;  
private:
  void refreshCategoriesDisplay(std::vector<std::string> visibleCategories
      , const size_t items);
  
private slots:
  void on_addCategory_button_clicked();
  
protected slots:
  virtual void on_nextProductPage_button_clicked() override;
  virtual void on_previousProductPage_button_clicked() override;
  virtual void on_deleteProduct_button_clicked() override;
  virtual void on_editProduct_button_clicked() override;
  virtual void on_categories_button_clicked() override;
  virtual void on_supplies_button_clicked() override;
  virtual void on_products_button_clicked() override;
};

#endif // CATEGORIES_H
