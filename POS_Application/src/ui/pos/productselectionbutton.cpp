#include "productselectionbutton.h"
#include "ui_productselectionbutton.h"

#include <QPainter>
#include <QPainterPath>
#include <QString>

ProductSelectionButton::ProductSelectionButton(QWidget *parent
    , const Product& myProduct)
    : QWidget(parent)
    , ui(new Ui::ProductSelectionButton)
    , product(myProduct) {
  ui->setupUi(this);
  
  this->ui->name_label->setText(this->product.getName().data());
  this->ui->name_label->adjustSize();
  QString price = QString::number(this->product.getPrice());
  this->ui->price_label->setText(price);
  this->ui->price_label->adjustSize();
}

ProductSelectionButton::~ProductSelectionButton() {
  delete ui;
}

void ProductSelectionButton::paintEvent(QPaintEvent* event) {
  QPixmap pixmap = this->product.getImage();
  // Dibuja el fondo solo si el pixmap no está vacío
  if (!pixmap.isNull()) {
    QPainter painter(this);
    // Establecer un clip con bordes redondeados
    QPainterPath path;
    path.addRoundedRect(rect(), 20, 20); // Cambia 20 por el radio de tus bordes
    painter.setClipPath(path);
    painter.drawPixmap(rect(), pixmap); // Escalar al tamaño del widget
  }
  QWidget::paintEvent(event);
}
