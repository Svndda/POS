#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LoginPage)
{
  ui->setupUi(this);
  connect(this->ui->sendCredentials_button, &QPushButton::clicked, this, []() {
    qDebug() << "Botón clicado!";
  });
  //connect(this->ui->sendCredentials_button, &QPushButton::clicked, this, &Login::on_sendCredentials_button_clicked);
}

Login::~Login()
{
  delete ui;
}

void Login::on_sendCredentials_button_clicked() {
  QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
  QRegularExpression passwordRegex("^[A-Za-z0-9]{8,}$");

  QString email = this->ui->email_lineEdit->text();
  QString password = this->ui->password_lineEdit->text();

  qDebug() << "Correo: " << email;
  qDebug() << "Contrasena: " << password;

  if (email.isEmpty() || password.isEmpty()) {
    QMessageBox::warning(this, "Error", "Porfavor, rellena todos los campos solicitados.");
  } else {
    QRegularExpressionMatch emailMatch = emailRegex.match(email);
    QRegularExpressionMatch passwordMatch = passwordRegex.match(password);

    if (emailMatch.hasMatch()) {
      if (passwordMatch.hasMatch()) {
        QMessageBox::information(this, "Exito", "Iniciando sesion.");
      } else {
        QMessageBox::warning(this, "Error", "Porfavor, introduzca una contraseña de al menos, 8 carácteres..");
      }
    } else {
      QMessageBox::warning(this, "Error", "Porfavor, introduzca un correo válido.");
    }
  }
}
