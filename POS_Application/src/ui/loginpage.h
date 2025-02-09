#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "posmodel.h"

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget {
  Q_OBJECT

public:
  explicit LoginPage(QWidget *parent = nullptr,
      POS_Model& model = POS_Model::getInstance());
  ~LoginPage();

private:
  Ui::LoginPage* ui = nullptr;
  // Reference to the application model.
  POS_Model& appModel;
  
signals:
  void sendCredentials(const User user);
private slots:
  void on_sendCredentials_button_clicked();
};

#endif // LOGIN_H
