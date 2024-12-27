#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class LoginPage;
}

class Login : public QWidget
{
  Q_OBJECT

public:
  explicit Login(QWidget *parent = nullptr);
  ~Login();

private:
  Ui::LoginPage *ui;

private slots:
  void on_sendCredentials_button_clicked();
};

#endif // LOGIN_H
