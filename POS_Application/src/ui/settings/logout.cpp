#include "logout.h"
#include "ui_logout.h"

#include <QMessageBox>

Logout::Logout(QWidget *parent, POS_Model& appmodel)
    : QWidget(parent)
    , ui(new Ui::Logout)
    , model(appmodel) {
  ui->setupUi(this);
  this->ui->contentLayout->setAlignment(Qt::AlignTop);
  this->connect(this->ui->acceptLogout_button, &QPushButton::clicked
      , this, &Logout::on_acceptLogout_clicked);

}

Logout::~Logout() {
  delete ui;
}

void Logout::on_acceptLogout_clicked() {
  qDebug() << "recibos actuales: " << this->model.getOngoingReceipts().size();
  if (this->model.getOngoingReceipts().size() > 0) {
    QMessageBox::warning(this, "Error en cierre de sesión"
    , "Por favor, cierre la caja actual.");
  } else {
    this->model.shutdown();
    emit this->acceptLogout_button_clicked();
  }
}
