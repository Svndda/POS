#include "pos.h"
#include "ui_pos.h"

#include <QMessageBox>

#include "productselectionbutton.h"
#include "receipt.h"
#include "receiptselectionbutton.h"

Pos::Pos(QWidget *parent, POS_Model& appModel)
    : QWidget(parent)
    , ui(new Ui::Pos)
    , model(appModel) {
  ui->setupUi(this);
  this->currentReceiptStack = new QStackedWidget();
  this->createSelectProductButtons(6);
  this->ui->billingLayout->setAlignment(Qt::AlignTop);
  Receipt* receipt = new Receipt(this, 0);
  this->ui->billingLayout->addWidget(this->currentReceiptStack);
  this->currentReceiptStack->addWidget(receipt);
  
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setAlignment(Qt::AlignLeft);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(1);
  this->ui->openedReceiptsArea->setLayout(layout);

  ReceiptSelectionButton* receiptSelectionButton
      = new ReceiptSelectionButton(this, this->openedReceipts);
  layout->addWidget(receiptSelectionButton);
}

Pos::~Pos() {
  delete ui;
}

void Pos::on_createReceipt_button_clicked() {
  QLayout* receiptsSelectButtonsLayout = this->ui->openedReceiptsArea->layout();
  if (receiptsSelectButtonsLayout->isEmpty()) {
    qDebug() << "anadiendo nueva factura";
    ReceiptSelectionButton* receiptSelectionButton
        = new ReceiptSelectionButton(this, ++this->openedReceipts);
    receiptsSelectButtonsLayout->addWidget(receiptSelectionButton);
    this->update();
  } else {
    QMessageBox::warning(this, "Error en solicitud"
        , "Por favor, cierre o finalice la factura actual.");
  }
}

void Pos::on_cancelReceipt_button_clicked() {
  qDebug() << "cancelando factura";
  if (this->currentReceiptStack) {
    QWidget* widget = this->currentReceiptStack->widget(0);
    this->currentReceiptStack->removeWidget(widget);
    widget->deleteLater();
    
    QLayoutItem* receiptSelecButton
        = this->ui->openedReceiptsArea->layout()->takeAt(0);
    receiptSelecButton->widget()->deleteLater();
    --this->openedReceipts;
        
    Receipt* receipt = new Receipt(this, 0);
    this->currentReceiptStack->addWidget(receipt);
    this->update();
  }
}

void Pos::addProductToReceipt(const Product& product) {
  qDebug() << "Anadiendo producto al recibo: " << product.getName();
  if (this->ui->openedReceiptsArea->layout()->count() > 0) {
    // Retrieve the currently active widget from the receiptStack  
    QWidget* currentWidget = this->currentReceiptStack->currentWidget();
    
    // Safely cast the QWidget to a Receipt pointer using qobject_cast
    Receipt* currentReceipt = qobject_cast<Receipt*>(currentWidget);
    
    if (currentReceipt) {
      // Assuming Receipt has a method addProduct(const Product&)
      currentReceipt->addProduct(product);
    } else {
      qDebug() << "Error: The current widget is not a Receipt.";
    }
  } else {
    QMessageBox::warning(this, "Error en la adición del producto"
        , "Por favor, primero cree una nueva factura.");
  }
}

void Pos::createSelectProductButtons(size_t numberOfProducts) {
  // Creates a layout to store the different product selection buttons.
  QGridLayout* layout = new QGridLayout(this->ui->products_WidgetContents);
  // Stablish the layout cont margins.
  layout->setContentsMargins(0, 10, 0, 10);
  // Sets the layout spacing.
  layout->setSpacing(10);
  // Sets the layout aligment to the top.
  layout->setAlignment(Qt::AlignTop);
  // Insert the layout into the widgets content of the scroll area for the
  // select product buttons.
  this->ui->products_WidgetContents->setLayout(layout);
  
  // Variables that calculates each widget coordinates.
  size_t x_position = 0;
  size_t y_position = 0;
  // Vector of the registered products in the model.
  std::vector<std::pair<std::string, Product>>& products
      = this->model.getRegisteredProductsVector();
  // Iterates through all the registered products, creating a selection button
  // for each one.
  for (const auto& product : products) {
    // Creates a product selection button for the actual product.
    ProductSelectionButton* button =
        new ProductSelectionButton(this, product.second);
    
    this->connect(button
        , &ProductSelectionButton::on_productSelectionButton_clicked
        , this
        , &Pos::addProductToReceipt);
    
    // Adds the widget to the container layout.
    layout->addWidget(button, y_position, x_position, Qt::AlignHCenter);
    
    ++x_position;
    if (x_position == numberOfProducts) {
      x_position = 0;
      ++y_position;
    }
  }
}
