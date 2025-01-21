#ifndef SUPPLIES_H
#define SUPPLIES_H

#include "inventory.h"

namespace Ui {
class Supplies;
}

class Supplies : public Inventory {
  Q_OBJECT
  
private:
  Ui::Supplies* ui;
  
public:
  explicit Supplies(QWidget *parent = nullptr
      , POS_Model& appModel = POS_Model::getInstance());
  ~Supplies();
  
protected:
  void refreshDisplay(const size_t pageItems) override;
  void setupConnections() override;  
private:
  void refreshCategoriesDisplay(std::vector<std::string> visibleCategories
      , const size_t items);
  
private slots:
  void on_addSupply_button_clicked();
  
protected slots:
  virtual void on_nextProductPage_button_clicked() override;
  virtual void on_previousProductPage_button_clicked() override;
  virtual void on_deleteProduct_button_clicked() override;
  virtual void on_editProduct_button_clicked() override;
  virtual void on_categories_button_clicked() override;
  virtual void on_supplies_button_clicked() override;
  virtual void on_products_button_clicked() override;
};

#endif // SUPPLIES_H
