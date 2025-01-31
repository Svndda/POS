#include "pos.h"
#include "ui_pos.h"

#include <QFile>

#include "ui_receiptelementwidget.h"
#include "productselectionbutton.h"

Pos::Pos(QWidget *parent, POS_Model& appModel)
    : QWidget(parent)
    , ui(new Ui::Pos)
    , model(appModel) {
  ui->setupUi(this);
  QVBoxLayout* layout = new QVBoxLayout(this->ui->receipt_WidgetContents);
  layout->setAlignment(Qt::AlignTop);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  this->ui->receipt_WidgetContents->setLayout(layout);
  
  QWidget* container = new QWidget();
  // Configurar el contenido del segundo widget
  Ui::ReceiptElement_Widget receiptElementUi;
  receiptElementUi.setupUi(container);
  
  // Crear el segundo widget base
  QWidget* container2 = new QWidget();
  
  // Configurar el contenido del segundo widget
  Ui::ReceiptElement_Widget receiptElementUi2;
  receiptElementUi2.setupUi(container2);
  
  layout->addWidget(container2);
  layout->addWidget(container);
  
  this->refreshSelectProductButtons(9);
}

Pos::~Pos() {
  delete ui;
}

void Pos::refreshDisplay() {
  
}

void Pos::refreshSelectProductButtons(size_t numberOfProducts) {
  QGridLayout* layout = new QGridLayout(this->ui->products_WidgetContents);
  layout->setContentsMargins(0, 10, 0, 10);
  layout->setSpacing(10);
  layout->setAlignment(Qt::AlignTop);
  this->ui->products_WidgetContents->setLayout(layout);
  
  size_t x_position = 0;
  size_t y_position = 0;
  std::vector<std::pair<std::string, Product>> products =
      this->model.getProductsForPage(0, this->model.getNumberOfProducts());
  
  for (const auto& product : products) {
    ProductSelectionButton* button =
        new ProductSelectionButton(this, product.second);
    
    layout->addWidget(button, y_position, x_position, Qt::AlignHCenter);
    
    ++x_position;
    if (x_position == 6) {
      x_position = 0;
      ++y_position;
    }
  }
}
