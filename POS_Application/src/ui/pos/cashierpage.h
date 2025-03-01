#ifndef CASHIERPAGE_H
#define CASHIERPAGE_H

#include <QWidget>

#include "posmodel.h"

namespace Ui {
class CashierPage;
}

class CashierPage : public QWidget {
  Q_OBJECT
  
private:
  Ui::CashierPage *ui;
  POS_Model& model;       ///< Reference to the POS_Model singleton.
  
public:
  explicit CashierPage(QWidget *parent = nullptr
      , POS_Model &appModel = POS_Model::getInstance());
  
  ~CashierPage();
};

#endif // CASHIERPAGE_H
