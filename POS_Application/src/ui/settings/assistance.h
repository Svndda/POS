#ifndef ASSISTANCE_H
#define ASSISTANCE_H

#include <QWidget>

#include "posmodel.h"

namespace Ui {
class Assistance;
}

class Assistance : public QWidget
{
  Q_OBJECT
  
private:
  Ui::Assistance *ui;
  POS_Model& model;
  
public:
  explicit Assistance(QWidget *parent = nullptr
      , POS_Model& model = POS_Model::getInstance());
  ~Assistance();
};

#endif // ASSISTANCE_H
