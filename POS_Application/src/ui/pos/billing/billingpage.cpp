#include "billingpage.h"
#include "ui_billingpage.h"

#include <QMessageBox>
#include <QTimer>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter.h>
#include <QPrinterInfo>

#include "productselectionbutton.h"
#include "order.h"
#include "orderselectionbutton.h"
#include "processorderdialog.h"
#include "util.h"

BillingPage::BillingPage(QWidget *parent, Model& appmodel)
    : QWidget(parent)
    , ui(new Ui::BillingPage)
    , model(appmodel) {
  ui->setupUi(this);
  this->setupDisplay();
  this->update();
}

BillingPage::~BillingPage() {
  delete ui;
}

void BillingPage::setupDisplay() {
  // Creates the widget stack to store the current orders.
  this->ordersStack = new QStackedWidget();
  // Sets the orders layout container alignment to the top.
  this->ui->billingLayout->setAlignment(Qt::AlignTop);
  // Sets the stack of orders as the widget.
  this->ui->billingLayout->addWidget(this->ordersStack);
  
  // Creates a layout to store the order selection buttons of the pos.
  QHBoxLayout* layout = new QHBoxLayout();
  // Sets the layout aligment to the left.
  layout->setAlignment(Qt::AlignLeft);
  // Sets the layout content margins to zero.
  layout->setContentsMargins(0, 0, 0, 0);
  // Sets the layout elements spacing to one.
  layout->setSpacing(1);
  // Sets the layout of the opened orders widget.
  this->ui->openedOrdersArea->setLayout(layout);
  
  // Adds a placeholder order.
  this->addOrder();
  
  // Creates the product selection buttons on the scroll view, with six elements
  // per row.
  this->createSelectProductButtons();
  
  // Connects the ui buttons to their slot functions.
  this->connect(this->ui->cancelOrder_button, &QPushButton::clicked
      , this, &BillingPage::cancelOrder_button_clicked);
  this->connect(this->ui->createOrder_button, &QPushButton::clicked
      , this, &BillingPage::createOrder_button_clicked);
  this->connect(this->ui->payOrder_button, &QPushButton::clicked
      , this, &BillingPage::payOrder_button_clicked);
}

void BillingPage::addOrder() {
  // Obtains the layout of orders selection buttons.
  QLayout* layout = this->ui->openedOrdersArea->layout();
  // Checks if the layout is empty.
  if (!layout->isEmpty()) {
    // Display a message box warning that there is an remaining order.
    QMessageBox::warning(this, "Error en solicitud"
        , "Por favor, cierre o finalice la orden actual.");
    return;
  }
  qDebug() << "anadiendo nueva orden";
  // Creates a new order and adds it into the order stack.
  Order* order = new Order;
  this->ordersStack->addWidget(order);
  // Creates a new order selection button and add it to the layout.
  OrderSelectionButton* orderSelectionButton
      = new OrderSelectionButton(this, ++this->openedOrders);
  layout->addWidget(orderSelectionButton);
  // Force the ui content to update.
  this->update();
}

void BillingPage::removeOrder(QWidget* orderWidget, const size_t index) {
  // Takes the order button from the layout.
  QLayoutItem* orderButton =
      this->ui->openedOrdersArea->layout()->takeAt(index);
  // Checks if the widget and order button pointers are valid.
  if (orderWidget && orderButton) {
    // Remove the order from the stack.
    ordersStack->removeWidget(orderWidget);
    // Force the widget deletion.
    orderWidget->deleteLater();
    
    // Remove the widget from the ui.
    delete orderButton->widget();
    // Deletes the button pointer.
    delete orderButton;
  }
  // Force the ui content to update.  
  this->update();
}

void BillingPage::createOrder_button_clicked() {
  // Checks if the user has access to do the action.
  if (this->model.getPageAccess(1) == User::PageAccess::EDITABLE) {
    // Checks if theres an opened cashier.
    if (!this->model.isCashierOpened()) {
      // Displays a warning message box to show the error to the user.
      QMessageBox::warning(this, "Error en order."
          , "Por favor, primero abra una nueva caja.");
      return;
    }
    this->addOrder();
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void BillingPage::cancelOrder_button_clicked() {
  // Checks if the user has access to do the action.
  if (this->model.getPageAccess(1) == User::PageAccess::EDITABLE) {
    // Checks if theres an opened cashier.
    if (!this->model.isCashierOpened()) {
      // Displays a warning message box to show the error to the user.
      QMessageBox::warning(this, "Error en order."
          , "Por favor, primero abra una nueva caja.");
      return;
    }
    // Removes the current order and creates a new one to be a placeholder.
    this->removeOrder(this->ordersStack->currentWidget(), 0);
    // Decrease the counter of opened orders.
    --this->openedOrders;
    this->addOrder();
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void BillingPage::payOrder_button_clicked() {
  if (this->model.getPageAccess(1) == User::PageAccess::EDITABLE) {
    // Checks if the orders stack isn't empty.
    if (this->ui->openedOrdersArea->layout()->count() > 0
        && this->ordersStack->widget(0)
        && this->model.isCashierOpened()) {
      // Obstains the widget of the first position of the stack.
      QWidget* widget = this->ordersStack->widget(0);
      if (widget) {
        // Cast the order widget from the stack.
        Order* order = qobject_cast<Order*>(widget);
        qDebug() << "precio del recibo: " << order->getPrice();
        // Checks if the order has products.
        if (order->getProducts().empty()) {
          return;
        }
        // Creates the dialog to process the order.
        ProcessOrderDialog processOrderDialog(this, *order);
        if (processOrderDialog.exec() == QDialog::Accepted) {
          // Prints the receipt for the order.
          this->printReceipt(*order);
          // Removes the order widget from the ui.
          this->removeOrder(widget, 0);
          qDebug() << "Rastreo: elimina el widget del selector de la orden.";
          // Adds a new order to be a placeholder.
          this->addOrder();
          // Generate a receipt data from the order and store it into de model.
          this->model.generateReceipt(*order);
          // Emits the signal that an order has been processed.
          emit this->orderProcessed();
          qDebug() << "Rastreo: Se emitio la senal";
        }
      }
    } 
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void BillingPage::printReceipt(const Order& order) {  
  auto printer = Util::systemPrinter(this, "POS-80C");
  
  // Paint the receipt with the given printer.
  this->paintReceipt(*printer, order);
  
  qDebug() << "Rastreo: sale de paintReceipt";
}

void BillingPage::paintReceipt(QPrinter& printer, const Order& order) {
  // Obtains a painter for the given printer.
  QPainter painter(&printer);
  if (!painter.isActive()) {
    qDebug() << "Error: Could not start painter.";
    return;
  }
  
  // Stablish the painter settings.
  const int margin = 5;
  const int printableWidth = printer.pageRect(QPrinter::DevicePixel).width();
  const int contentWidth = printableWidth - (2 * margin);
  
  // Sets the fonts used fro the receipt printing.
  QFont fontNormal("Courier", 10);
  QFont fontBold("Courier", 10, QFont::Bold);
  painter.setFont(fontNormal);
  
  QFontMetrics metrics(fontNormal);
  const int lineHeight = metrics.height();
  int y = 10;
  
  // Print receipt header
  Util::printWrappedLine(
      painter, "Macana's Place", y, true, metrics, 
      margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, "Nº Recibo: " + QString::number(model.getNextReceiptID()), y, 
      false, metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, "Usuario: "
      + QString(model.getCurrentUser().getUsername().data())
      , y, false, metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, "Fecha: " + 
      QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"), 
      y, false, metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, "==============================", y, 
      false, metrics, margin, contentWidth, lineHeight
  );
  
  // Print order items
  for (const auto& product : order.getProducts()) {
    Util::printWrappedLine(
        painter, QString("%1 x %2")
        .arg(product.second)
        .arg(product.first.getName().data()), y
        , false, metrics, margin, contentWidth, lineHeight
    );
    Util::printWrappedLine(
        painter, QString("₡%1").arg(product.first.getPrice()), 
        y, false, metrics, margin, contentWidth, lineHeight
    );
  }
  
  // Print receipt footer
  Util::printWrappedLine(
      painter, "==============================", y, 
      false, metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, "Cantidad de artículos: " + QString::number(
      order.getProducts().size()), y, false, metrics
      , margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, "==============================", y, 
      false, metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, "Monto total: ₡" + QString::number(order.getPrice(), 'f', 2)
      , y, true, metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, "==============================", y, 
      false, metrics, margin, contentWidth, lineHeight
  );
  
  // Print payment information
  const double receivedMoney = order.getReceivedMoney();
  const double changeMoney = receivedMoney - order.getPrice();
  Util::printWrappedLine(
      painter, "Método de pago: " + order.getPaymentMethod(), y, 
      false, metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, "Recibido: ₡" + QString::number(receivedMoney, 'f', 2), y, 
      false, metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, "Dinero a entregar: ₡" + QString::number(changeMoney, 'f', 2), y, 
      false, metrics, margin, contentWidth, lineHeight
  );
  
  painter.end();
}

void BillingPage::createSelectProductButtons() {
  // Creates a layout to store the different product selection buttons.
  QGridLayout* layout = new QGridLayout(this->ui->products_WidgetContents);
  // Stablish the layout cont margins.
  layout->setContentsMargins(0, 10, 0, 10);
  // Sets the layout spacing.
  layout->setSpacing(10);
  // Sets the layout aligment to the top.
  layout->setAlignment(Qt::AlignTop);

  // this->ui->products_widget->updateGeometry();
  // this->ui->products_widget->adjustSize();
  // this->ui->productSelectionButtonsScrollArea->updateGeometry();
  // this->ui->productSelectionButtonsScrollArea->adjustSize();
  // this->ui->products_WidgetContents->updateGeometry();
  // this->ui->products_WidgetContents->adjustSize();
  
  // const int containerWidth = this->ui->products_widget->heigth();
  
  // // Calcular cuántos botones caben horizontalmente.
  // const int buttonWidth = 160;
  // const int spacing = 10;
  // int numberOfProducts = (containerWidth + spacing) / (buttonWidth + spacing);
  const size_t numberOfProducts = 4;
    
  // Variables that calculates each widget coordinates.
  size_t x_position = 0;
  size_t y_position = 0;
  
  // Vector of the registered products in the model.
  const std::vector<std::pair<std::string, Product>>& products
      = this->model.getRegisteredProductsVector();
  
  // Iterates through all the registered products, creating a selection button
  // for each one.
  for (const auto& product : products) {
    // Creates a product selection button for the actual product.
    ProductSelectionButton* button =
        new ProductSelectionButton(this, product.second);
    
    // Connects the button clicked signal with the pos slot ot handle it.
    this->connect(button
        , &ProductSelectionButton::on_productSelectionButton_clicked
        , this, &BillingPage::addProductToOrder);
    
    // Adds the widget to the container layout.
    layout->addWidget(button, y_position, x_position);
    
    // Calculates the next button position indexes.
    ++x_position;
    if (x_position == numberOfProducts) {
      x_position = 0;
      ++y_position;
    }
  }
}

void BillingPage::addProductToOrder(const Product& product) {
  // Checks if the user has editable access of this page.
  if (this->model.getPageAccess(1) != User::PageAccess::EDITABLE) {
    // Displays a warning message box to show the error to the user.    
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
    return;
  }
  // Checks if theres a cashier opened.
  if (!this->model.isCashierOpened()) {
    // Displays a warning message box to show the error to the user.
    QMessageBox::warning(this, "Error en order."
        , "Por favor, primero abra una nueva caja.");
    return;
  }
  // Checks that there's al least 1 order opened to accept the click from a
  // product selection button.
  if (this->ui->openedOrdersArea->layout()->count() <= 0) {
    // Displays a warning message box to show the error to the user.
    QMessageBox::warning(this, "Error en la adición del producto"
        , "Por favor, primero cree una nueva factura.");
  }
  // Retrieve the currently active widget from the orderStack  
  QWidget* orderWidget = this->ordersStack->currentWidget();
  // Safely cast the QWidget to a Order pointer using qobject_cast
  Order* order = qobject_cast<Order*>(orderWidget);
  
  // Checks that the current order pointer isn't null.
  if (order) {
    // Adds the product into the current order.
    order->addProduct(product);
  } else {
    qDebug() << "Error: The current widget is not a Order.";
  }
}
