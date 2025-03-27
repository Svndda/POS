#include "productselectionbutton.h"
#include "ui_productselectionbutton.h"

#include <QPainter>
#include <QPainterPath>
#include <QString>

ProductSelectionButton::ProductSelectionButton(QWidget* parent
    , const Product& myProduct)
    : QWidget(parent)
    , ui(new Ui::ProductSelectionButton)
    , product(myProduct) {
  this->ui->setupUi(this);
  this->refreshDisplay();
}

ProductSelectionButton::~ProductSelectionButton() {
  delete ui;
}

void ProductSelectionButton::refreshDisplay() {
  // Updates/sets the product's infroamtion as the value of the ui labels.
  this->ui->name_label->setText(this->product.getName().data());
  this->ui->name_label->adjustSize();
  QString price = QString::number(this->product.getPrice());
  this->ui->price_label->setText(price);
  this->ui->price_label->adjustSize();
  this->update();
}

void ProductSelectionButton::paintEvent(QPaintEvent* event) {
  // Obstains product's pixmap.
  QPixmap pixmap = this->product.getImage();
  // Paints the background only if the pixmap contains information.
  if (!pixmap.isNull()) {
    // Creates a painter for this ui.
    QPainter painter(this);
    // Stablish a clip of 20 redius.
    QPainterPath path;
    path.addRoundedRect(rect(), 20, 20);
    painter.setClipPath(path);
    painter.drawPixmap(rect(), pixmap); // Escalar al tamaño del widget
  }
  QWidget::paintEvent(event);
  this->update();
}

void ProductSelectionButton::on_button_clicked() {
  // Emits the signal indicating that the button has been clicked.
  emit on_productSelectionButton_clicked(this->product);
}
