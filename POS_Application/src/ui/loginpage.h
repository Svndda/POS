#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "model.h"

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget {
  Q_OBJECT

public:
  explicit LoginPage(QWidget *parent = nullptr,
      Model& model = Model::getInstance());
  ~LoginPage();

private:
  Ui::LoginPage* ui = nullptr;
  // Reference to the application model.
  Model& appModel;
  
signals:
  void sendCredentials(const User user);
private slots:
  void on_sendCredentials_button_clicked();
};

#endif // LOGIN_H
