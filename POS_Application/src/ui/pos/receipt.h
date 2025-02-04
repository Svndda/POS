#ifndef RECEIPT_H
#define RECEIPT_H

#include <QWidget>

#include "product.h"

namespace Ui {
class Receipt;
}

class Receipt : public QWidget {
  Q_OBJECT
  
private:
  Ui::Receipt *ui;
  const size_t ID = 0;
  
public:
  explicit Receipt(QWidget *parent = nullptr, const size_t myID = 0);
  ~Receipt();
  
public:
  void addProduct(const Product& product);
};

#endif // RECEIPT_H
