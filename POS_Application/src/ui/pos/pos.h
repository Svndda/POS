#ifndef POS_H
#define POS_H

#include <QWidget>
#include <QStackedWidget>

#include "posmodel.h"

namespace Ui {
class Pos;
}

class Pos : public QWidget {
  Q_OBJECT
  
private:
  Ui::Pos* ui;
  POS_Model& model;
  QStackedWidget* currentReceiptStack;
  size_t openedReceipts = 1;
  
public:
  void extracted();
  explicit Pos(QWidget *parent = nullptr,
      POS_Model &appModel = POS_Model::getInstance());
  ~Pos();
  
  void addProductToReceipt(const Product& product);
  void createSelectProductButtons(size_t numberOfProducts);
  
private slots:
  void on_createReceipt_button_clicked();
  void on_cancelReceipt_button_clicked();
};

#endif // POS_H
