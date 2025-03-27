#ifndef LOGOUT_H
#define LOGOUT_H

#include <QWidget>

#include "posmodel.h"

namespace Ui {
class Logout;
}

class Logout : public QWidget
{
  Q_OBJECT
  
private:
  Ui::Logout *ui;
  POS_Model& model;

public:
  explicit Logout(QWidget *parent = nullptr
      , POS_Model& model = POS_Model::getInstance());
  ~Logout();
  
private slots:
  void on_acceptLogout_clicked();
  
signals:
  void acceptLogout_button_clicked();
};

#endif // LOGOUT_H
