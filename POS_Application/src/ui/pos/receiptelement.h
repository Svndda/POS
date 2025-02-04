#ifndef RECEIPTELEMENT_H
#define RECEIPTELEMENT_H

#include <QWidget>

#include "product.h"

namespace Ui {
class ReceiptElement;
}

class ReceiptElement : public QWidget {
  Q_OBJECT
  
private:
  Ui::ReceiptElement* ui;
  const Product& product;
  size_t quantity = 1;
  double totalPrice = 0;
  
public:
  explicit ReceiptElement(QWidget *parent, const Product& myProduct);
  ~ReceiptElement();
  
private:
  void deleteSelf();
  
private slots:
  void incrementQuantity_button_clicked();
  void reduceQuantity_button_clicked();
};

#endif // RECEIPTELEMENT_H
