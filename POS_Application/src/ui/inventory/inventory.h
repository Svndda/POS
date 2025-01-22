#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>
#include <QPushButton>

#include "posmodel.h"

class Inventory : public QWidget {
  Q_OBJECT
  
public:
  explicit Inventory(QWidget* parent = nullptr
      , POS_Model& appModel = POS_Model::getInstance()
      , const size_t items = 9)
      : QWidget(parent)
      , model(appModel)
      , currentPageIndex(0)
      , itemsPerPage(items) {}
  
  virtual ~Inventory() {}
  
protected:
  POS_Model& model;
  size_t currentPageIndex = 0;
  size_t itemsPerPage = 9;
  
protected:
  virtual void setupConnections() = 0;
  virtual void refreshDisplay(const size_t pageItems) = 0;
  
public:
signals:
  void products_button_signal();
  void categories_button_signal();
  void supplies_button_signal();
  
protected slots:
  virtual void on_nextPage_button_clicked() {};
  virtual void on_previousPage_button_clicked() {};
  virtual void on_delete_button_clicked() {};
  virtual void on_edit_button_clicked() {};
  virtual void on_categories_button_clicked() {};
  virtual void on_supplies_button_clicked() {};
  virtual void on_products_button_clicked() {};
};

#endif // INVENTORY_H
