#ifndef PERSONALIZATION_H
#define PERSONALIZATION_H

#include <QWidget>

#include "model.h"

namespace Ui {
class Personalization;
}

class Personalization : public QWidget {
  Q_OBJECT
  
private:
  Ui::Personalization *ui;
  Model& model;
  
public:
  explicit Personalization(QWidget *parent = nullptr
      , Model& model = Model::getInstance());
  ~Personalization();

};

#endif // PERSONALIZATION_H
