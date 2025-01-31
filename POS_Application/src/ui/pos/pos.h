#ifndef POS_H
#define POS_H

#include <QWidget>

#include "posmodel.h"

namespace Ui {
class Pos;
}

class Pos : public QWidget {
  Q_OBJECT
  
private:
  Ui::Pos *ui;
  POS_Model& model;  
  
public:
  explicit Pos(QWidget *parent = nullptr
      , POS_Model& appModel = POS_Model::getInstance());
  ~Pos();
  
  void refreshDisplay();
  void refreshSelectProductButtons(size_t numberOfProducts);
};

#endif // POS_H
