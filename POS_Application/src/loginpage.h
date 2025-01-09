#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "appmodel.h"

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget {
  Q_OBJECT

public:
  explicit LoginPage(QWidget *parent = nullptr,
      AppModel& model = AppModel::getInstance());
  ~LoginPage();

private:
  Ui::LoginPage *ui;
  // Reference to the application model.
  AppModel& appModel;

private slots:
  void on_sendCredentials_button_clicked();
};

#endif // LOGIN_H
