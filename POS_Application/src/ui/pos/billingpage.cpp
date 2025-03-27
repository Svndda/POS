#include "billingpage.h"
#include "ui_billingpage.h"

#include <QMessageBox>
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

BillingPage::BillingPage(QWidget *parent, POS_Model& appmodel)
    : QWidget(parent)
    , ui(new Ui::BillingPage)
    , model(appmodel) {
  ui->setupUi(this);
  this->setupDisplay();
}

BillingPage::~BillingPage() {
  delete ui;
}

void BillingPage::setupDisplay() {
  // Creates the widget stack to store the current orders.
  this->ordersStack = new QStackedWidget();
  // Creates the product selection buttons on the scroll view, with six elements
  // per row.
  this->createSelectProductButtons(5);
  // Sets the orders layout container alignment to the top.
  this->ui->billingLayout->setAlignment(Qt::AlignTop);
  
  // Creates a new order.
  Order* order = new Order(this);
  // Adds the new order to the layout.
  this->ui->billingLayout->addWidget(this->ordersStack);
  // Adds the nre order to the stak.
  this->ordersStack->addWidget(order);
  
  // Creates a layout to store the order selection buttons of the pos.
  QHBoxLayout* layout = new QHBoxLayout();
  // Sets the layout aligment to the left.
  layout->setAlignment(Qt::AlignLeft);
  // Sets the layout content margins to zero.
  layout->setContentsMargins(0, 0, 0, 0);
  // Sets the layout elements spacing to one.
  layout->setSpacing(1);
  this->ui->openedOrdersArea->setLayout(layout);
  
  // Creates and adds up a order selection button into the layout.
  OrderSelectionButton* orderSelectionButton
      = new OrderSelectionButton(this, ++this->openedOrders);
  layout->addWidget(orderSelectionButton);
}

void BillingPage::on_createOrder_button_clicked() {
  if (this->model.getPageAccess(1) == User::PageAccess::EDITABLE) {
    if (this->model.isCashierOpened()) {
      // Obtains the layout of orders selection buttons.
      QLayout* ordersSelectButtonsLayout = this->ui->openedOrdersArea->layout();
      // Checks if the layout is empty.
      if (ordersSelectButtonsLayout->isEmpty()) {
        qDebug() << "anadiendo nueva factura";
        // Creates a new order selection button and add it to the layout.
        OrderSelectionButton* orderSelectionButton
            = new OrderSelectionButton(this, ++this->openedOrders);
        ordersSelectButtonsLayout->addWidget(orderSelectionButton);
        // Force the ui content to uodate.
        this->update();
      } else {
        // Display a message box warning that there is an remaining order.
        QMessageBox::warning(this, "Error en solicitud"
            , "Por favor, cierre o finalice la factura actual.");
      }
    } else {
      // Displays a warning message box to show the error to the user.
      QMessageBox::warning(this, "Error en order."
          , "Por favor, primero abra una nueva caja.");
    }
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void BillingPage::on_cancelOrder_button_clicked() {
  if (this->model.getPageAccess(1) == User::PageAccess::EDITABLE) {
    // Checks if the order stack isn't empty.
    if (this->ui->openedOrdersArea->layout()->count() > 0
        && this->ordersStack->widget(0)
        && this->model.isCashierOpened()) {
      
      qDebug() << "stack: " << this->ordersStack->count();
      qDebug() << "cashier: " << this->model.isCashierOpened();

      QWidget* widget = this->ordersStack->widget(0);
      if (widget) {
        // Remove the order widget from the stack.
        this->ordersStack->removeWidget(widget);
        // Force the object deletion.        
        widget->deleteLater();
      }
      
      // Verificar que el layout de openedOrdersArea exista antes de modificarlo
      if (this->ui->openedOrdersArea->layout()) {
        QLayoutItem* orderSelectButton
            = this->ui->openedOrdersArea->layout()->takeAt(0);
        if (orderSelectButton && orderSelectButton->widget()) {
          orderSelectButton->widget()->deleteLater();
          delete orderSelectButton;
        }
      }
      
      if (this->openedOrders > 0) {
        --this->openedOrders;
      }
      // Creates a new empty order to be a place holder.
      if (this->ordersStack->count() == 0) {
        Order* order = new Order(this);
        // Adds the new order to the related stack.        
        this->ordersStack->addWidget(order);
      }
      // For the ui update.
      this->update();
    }
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void BillingPage::on_payOrder_button_clicked() {
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
        qDebug() << "precio del recibo: " << order->getOrderPrice();
        if (order->getOrderProducts().empty()) {
          return;
        }
        
        ProcessOrderDialog processOrderDialog(this, *order);
        if (processOrderDialog.exec() == QDialog::Accepted) {
          this->printReceipt(*order);
          
          // Eliminar la orden impresa de la pila
          this->ordersStack->removeWidget(widget);
          widget->setParent(nullptr);
          widget->deleteLater();
          
          qDebug() << "Rastreo: elimina el widget de la orden";
          
          // Eliminar el botón de selección asociado a la orden
          QLayoutItem* orderSelectButton = this->ui->openedOrdersArea->layout()->takeAt(0);
          if (orderSelectButton) {
            if (QWidget* btn = orderSelectButton->widget()) {
              btn->deleteLater();
            }
            delete orderSelectButton;
          }
          
          qDebug() << "Rastreo: elimina el widget del selector de la orden.";
          
          // Crear una nueva orden en la pila
          Order* order2 = new Order(this);
          this->ordersStack->addWidget(order2);
          
          this->on_createOrder_button_clicked();
          
          qDebug() << "Rastreo: crea una nueva vista de orden de la orden";
          
          // Actualizar la UI
          this->update();
          
          this->model.generateReceipt(*order);
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
  QList<QPrinterInfo> printers = QPrinterInfo::availablePrinters();

  QPrinter printer;
  QString macanasPosPrinter = "POS-80C";
  for (const QPrinterInfo& printerIt : printers) {
    if (printerIt.printerName() == macanasPosPrinter) {
      qDebug() << "✅ La impresora" << macanasPosPrinter << "está disponible.";
      printer.setPrinterName(macanasPosPrinter);
    }
  }
  
  qDebug() << "nombre de la impresora: " << printer.printerName();
  if (printer.printerName() != macanasPosPrinter) {
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() != QDialog::Accepted) {
      qDebug() << "Impresión cancelada por el usuario.";
      return;
    }
  }

  // Verificar que la impresora seleccionada es válida
  if (!printer.isValid()) {
    QMessageBox::critical(this, "Error", "No se encontró una impresora válida.");
    return;
  }
  
  // Si el usuario seleccionó una impresora PDF, pedirle un archivo de salida
  if (printer.printerName().contains("PDF", Qt::CaseInsensitive)) {
    QString filePath = QFileDialog::getSaveFileName(this
        , "Guardar Recibo como PDF", "", "Archivos PDF (*.pdf)");
    if (filePath.isEmpty()) {
      qDebug() << "El usuario canceló la selección del archivo.";
      return;
    }
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
  } else {
    printer.setOutputFormat(QPrinter::NativeFormat);
  }

  qDebug() << "Printer Output File: " << printer.outputFileName();
  qDebug() << "Printer Resolution: " << printer.resolution();
  qDebug() << "Printer Format: " << (printer.outputFormat() == QPrinter::PdfFormat ? "PDF" : "Otro");

  printer.setResolution(84);
  printer.setPageSize(QPageSize(QSizeF(48, 500), QPageSize::Millimeter));
  printer.setPageMargins(QMarginsF(0, 0, 0, 0), QPageLayout::Millimeter);
  printer.setFullPage(true);
  
  // Pintar el recibo
  this->paintReceipt(printer, order);
  
  qDebug() << "Rastreo: sale de paintReceipt";
}

void BillingPage::paintReceipt(QPrinter& printer, const Order& order) {
  QPainter painter(&printer);
  if (!painter.isActive()) {
    qDebug() << "Error: No se pudo iniciar el pintor.";
    return;
  }
  
  int printableWidth = printer.pageRect(QPrinter::DevicePixel).width();
  int margin = 5;
  int contentWidth = printableWidth - (2 * margin);
  
  QFont fontNormal("Courier", 10);
  QFont fontBold("Courier", 10, QFont::Bold);
  
  painter.setFont(fontNormal);
  QFontMetrics metrics(fontNormal);
  int lineHeight = metrics.height();
  
  auto printWrappedLine = [&](const QString& text, int& y, bool bold = false) {
    painter.setFont(bold ? fontBold : fontNormal);
    QStringList wrappedLines;
    QString currentLine;
    
    for (const QChar& c : text) {
      currentLine += c;
      if (metrics.horizontalAdvance(currentLine) > contentWidth) {
        wrappedLines.append(currentLine);
        currentLine.clear();
      }
    }
    
    if (!currentLine.isEmpty()) {
      wrappedLines.append(currentLine);
    }
    
    for (const QString& line : wrappedLines) {
      painter.drawText(margin, y, line);
      y += lineHeight;
    }
  };
  
  int y = 10;
  printWrappedLine("Macana's Place", y, true);
  printWrappedLine("Nº Recibo: "
      + QString::number(this->model.getNextReceiptID()), y);
  printWrappedLine("Usuario: "
      + QString(this->model.getCurrentUser().getUsername().data()), y);
  printWrappedLine("Fecha: "
      + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"), y);
  printWrappedLine("==============================", y);
  
  for (const auto& product : order.getOrderProducts()) {
    printWrappedLine(QString("%1 x %2")
                      .arg(product.second)
                      .arg(product.first.getName().data()), y);
    printWrappedLine(QString("₡%1").arg(product.first.getPrice()), y);
  }
  
  printWrappedLine("==============================", y);
  printWrappedLine("Cantidad de artículos: "
      + QString::number(order.getOrderProducts().size()), y);
  printWrappedLine("==============================", y);
  printWrappedLine("Monto total: ₡"
      + QString::number(order.getOrderPrice(), 'f', 2), y, true);
  printWrappedLine("==============================", y);
  
  double receivedMoney = order.getReceivedMoney();
  double changeMoney = receivedMoney - order.getOrderPrice();
  printWrappedLine("Método de pago: " + order.getPaymentMethod(), y);
  printWrappedLine("Recibido: ₡" + QString::number(receivedMoney, 'f', 2), y);
  printWrappedLine("Dinero a entregar: ₡"
      + QString::number(changeMoney, 'f', 2), y);
  
  painter.end();
}

void BillingPage::createSelectProductButtons(size_t numberOfProducts) {
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
    
    // Connects the button clicked signal with the pos slot ot handle it.
    this->connect(button
        , &ProductSelectionButton::on_productSelectionButton_clicked
        , this, &BillingPage::addProductToOrder);
    
    // Adds the widget to the container layout.
    layout->addWidget(button, y_position, x_position, Qt::AlignHCenter);
    
    // Calculates the next button position indexes.
    ++x_position;
    if (x_position == numberOfProducts) {
      x_position = 0;
      ++y_position;
    }
  }
}

void BillingPage::addProductToOrder(const Product& product) {
  if (this->model.getPageAccess(1) == User::PageAccess::EDITABLE) {
    // Checks that there's al least 1 order openend to accept the click from a
    // product selection button.
    if (this->model.isCashierOpened()) {
      if (this->ui->openedOrdersArea->layout()->count() > 0) {
        // Retrieve the currently active widget from the orderStack  
        QWidget* currentWidget = this->ordersStack->currentWidget();
        
        // Safely cast the QWidget to a Order pointer using qobject_cast
        Order* currentOrder = qobject_cast<Order*>(currentWidget);
        
        // Checks that the current order pointer isn't null.
        if (currentOrder) {
          // Adds the product into the current order.
          currentOrder->addProduct(product);
        } else {
          qDebug() << "Error: The current widget is not a Order.";
        }
      } else {
        // Displays a warning message box to show the error to the user.
        QMessageBox::warning(this, "Error en la adición del producto"
            , "Por favor, primero cree una nueva factura.");
      }
    } else {
      // Displays a warning message box to show the error to the user.
      QMessageBox::warning(this, "Error en order."
          , "Por favor, primero abra una nueva caja.");
    }
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}
