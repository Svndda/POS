#ifndef ASSISTANCE_H
#define ASSISTANCE_H

#include <QWidget>

#include "model.h"

namespace Ui {
class Assistance;
}

class Assistance : public QWidget
{
  Q_OBJECT
  
private:
  Ui::Assistance *ui;
  Model& model;
  
public:
  explicit Assistance(QWidget *parent = nullptr
      , Model& model = Model::getInstance());
  ~Assistance();
};

#endif // ASSISTANCE_H
