#ifndef PERSONALIZATION_H
#define PERSONALIZATION_H

#include <QWidget>

#include "posmodel.h"

namespace Ui {
class Personalization;
}

class Personalization : public QWidget {
  Q_OBJECT
  
private:
  Ui::Personalization *ui;
  POS_Model& model;
  
public:
  explicit Personalization(QWidget *parent = nullptr
      , POS_Model& model = POS_Model::getInstance());
  ~Personalization();

};

#endif // PERSONALIZATION_H
