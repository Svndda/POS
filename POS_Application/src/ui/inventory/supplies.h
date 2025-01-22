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
  void refreshSuppliesDisplay(const std::vector<SupplyItem>& visibleSupplies
      , const size_t items);
  
private slots:
  void addSupply_button_clicked();
  
protected slots:
  virtual void on_nextPage_button_clicked() override;
  virtual void on_previousPage_button_clicked() override;
  virtual void on_delete_button_clicked() override;
  virtual void on_edit_button_clicked() override;
  virtual void on_categories_button_clicked() override;
  virtual void on_supplies_button_clicked() override;
  virtual void on_products_button_clicked() override;
};

#endif // SUPPLIES_H
