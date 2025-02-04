#ifndef PRODUCTSELECTIONBUTTON_H
#define PRODUCTSELECTIONBUTTON_H

#include <QWidget>

#include "product.h"

namespace Ui {
class ProductSelectionButton;
}

class ProductSelectionButton : public QWidget {
  Q_OBJECT
  
private:
  Ui::ProductSelectionButton *ui;
  
public:
  const Product& product;
  
public:
  explicit ProductSelectionButton(QWidget *parent
      , const Product& myProduct);
  ~ProductSelectionButton();
  
protected:
  void paintEvent(QPaintEvent* event) override;
  
signals:
  void on_productSelectionButton_clicked(const Product& product);
  
private slots:
  void on_button_clicked();
};

#endif // PRODUCTSELECTIONBUTTON_H
