#ifndef PRODUCTFORMDIALOG_H
#define PRODUCTFORMDIALOG_H

#include <QDialog>
#include <map>
#include <string>
#include "product.h"

namespace Ui {
class ProductFormDialog;
}

class ProductFormDialog : public QDialog {
  Q_OBJECT
  
private:
  Ui::ProductFormDialog *ui;
  std::map<std::string, std::vector<Product>> registeredDrinks;
  std::map<std::string, std::vector<Product>> registeredDishes;
  Product createdProduct = Product(); 

public:
  explicit ProductFormDialog(QWidget *parent = nullptr,
      std::map<std::string, std::vector<Product>> drinks
          = std::map<std::string, std::vector<Product>>(),
      std::map<std::string, std::vector<Product>> dishes
          = std::map<std::string, std::vector<Product>>());
  ~ProductFormDialog();
  
  Product getProduct();
  
  QString getProductCategory();  
  
private slots:
  void on_acceptProduct_button_clicked();
  void on_cancelProduct_button_clicked();
};

#endif // PRODUCTFORMDIALOG_H
