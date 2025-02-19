#include "pos.h"
#include "ui_pos.h"

#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QPrintDialog>

#include "productselectionbutton.h"
#include "receipt.h"
#include "receiptselectionbutton.h"

Pos::Pos(QWidget *parent, POS_Model& appModel)
    : QWidget(parent)
    , ui(new Ui::Pos)
    , model(appModel) {
  ui->setupUi(this);
  this->setupDisplay();
}

Pos::~Pos() {
  delete ui;
}

void Pos::setupDisplay() {
  // Creates the widget stack to store the current receipts.
  this->receipStack = new QStackedWidget();
  // Creates the product selection buttons on the scroll view, with six elements
  // per row.
  this->createSelectProductButtons(6);
  // Sets the receipts layout container alignment to the top.
  this->ui->billingLayout->setAlignment(Qt::AlignTop);
  
  // Creates a new receipt.
  Receipt* receipt = new Receipt(this, 0);
  // Adds the new receipt to the layout.
  this->ui->billingLayout->addWidget(this->receipStack);
  // Adds the nre receipt to the stak.
  this->receipStack->addWidget(receipt);
  
  // Creates a layout to store the receipt selection buttons of the pos.
  QHBoxLayout* layout = new QHBoxLayout();
  // Sets the layout aligment to the left.
  layout->setAlignment(Qt::AlignLeft);
  // Sets the layout content margins to zero.
  layout->setContentsMargins(0, 0, 0, 0);
  // Sets the layout elements spacing to one.
  layout->setSpacing(1);
  this->ui->openedReceiptsArea->setLayout(layout);
  
  // Creates and adds up a receipt selection button into the layout.
  ReceiptSelectionButton* receiptSelectionButton
      = new ReceiptSelectionButton(this, this->openedReceipts);
  layout->addWidget(receiptSelectionButton);
}

void Pos::on_createReceipt_button_clicked() {
  if (this->model.getPageAccess(2) == User::PageAccess::EDITABLE) {
    // Obtains the layout of receipts selection buttons.
    QLayout* receiptsSelectButtonsLayout = this->ui->openedReceiptsArea->layout();
    // Checks if the layout is empty.
    if (receiptsSelectButtonsLayout->isEmpty()) {
      qDebug() << "anadiendo nueva factura";
      // Creates a new receipt selection button and add it to the layout.
      ReceiptSelectionButton* receiptSelectionButton
          = new ReceiptSelectionButton(this, ++this->openedReceipts);
      receiptsSelectButtonsLayout->addWidget(receiptSelectionButton);
      // Force the ui content to uodate.
      this->update();
    } else {
      // Display a message box warning that there is an remaining receipt.
      QMessageBox::warning(this, "Error en solicitud"
          , "Por favor, cierre o finalice la factura actual.");
    }
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void Pos::on_cancelReceipt_button_clicked() {
  if (this->model.getPageAccess(2) == User::PageAccess::EDITABLE) {
    // Checks if the receipt stack isn't empty.
    if (this->receipStack) {
      // Obtains the widget indexed on the first position of the current receipts
      // stack.
      QWidget* widget = this->receipStack->widget(0);
      // Remove the receipt widget from the stack.
      this->receipStack->removeWidget(widget);
      // Force the object deletion.
      widget->deleteLater();
      
      // Obtains the receipt's selection button.
      QLayoutItem* receiptSelectButton
          = this->ui->openedReceiptsArea->layout()->takeAt(0);
      // Force the object deletion.
      receiptSelectButton->widget()->deleteLater();
      // Decrement the openen receipts counter.
      --this->openedReceipts;
      // Creates a new empty receipt to be a place holder.
      Receipt* receipt = new Receipt(this, 0);
      // Adds the new receipt to the related stack.
      this->receipStack->addWidget(receipt);
      // For the ui update.
      this->update();
    } 
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void Pos::on_payReceipt_button_clicked() {
  if (this->model.getPageAccess(2) == User::PageAccess::EDITABLE) {
    // Checks if the receipts stack isn't empty.
    if (this->receipStack) {
      // Obstains the widget of the first position of the stack.
      QWidget* widget = this->receipStack->widget(0);
      // Creates a printer object to sets the receipt printint settings.
      QPrinter printer(QPrinter::HighResolution);
      // Sets the printing page size of the receipt.
      printer.setPageSize(QPageSize(QPageSize::A6));
      // Sets the printing ouput format of the receipt.
      printer.setOutputFormat(QPrinter::NativeFormat);
      // Sets the printing page margins of the receipt.
      printer.setPageMargins(QMarginsF(0, 0, 0, 0), QPageLayout::Millimeter);
      
      // Creates the print preview dialog.
      QPrintPreviewDialog previewDialog(&printer, this);
      
      // Connects the printing fucntion to the printer.
      connect(&previewDialog, &QPrintPreviewDialog::paintRequested
              , this, &Pos::printPreview);
      
      // Checks if the user printed the receipt.
      if (previewDialog.exec() == QDialog::Accepted) {
        // Remove the current receipt from the stack.
        this->receipStack->removeWidget(widget);
        // Force the deletion of the receipt.
        widget->deleteLater();
        
        // Obtains the receipt selection button related to the deleted receipt.
        QLayoutItem* receiptSelectButton
            = this->ui->openedReceiptsArea->layout()->takeAt(0);
        // Force the deletion of the button.
        receiptSelectButton->widget()->deleteLater();
        // Decrements the opened receipts counter.
        --this->openedReceipts;
        
        // Creates a new receipt to be a place holder.
        Receipt* receipt = new Receipt(this, 0);
        // Adds the new receipt to the stack.
        this->receipStack->addWidget(receipt);
        // Force the ui update.
        this->update();
      }
    } 
  } else {
    QMessageBox::information(this, "Acceso restrido."
                             , "El usuario no posee los permisos de edición.");
  }
}

void Pos::printPreview(QPrinter* printer) {
  // Obstains the receipt on the first position of the stack.
  QWidget* widget = this->receipStack->widget(0);
  // Cast the polymorph widget pouinter into a receipt pointer.
  Receipt* receipt = qobject_cast<Receipt*>(widget);
  
  // Checks that the receipt pointer isn't null.
  if (!receipt) {
    qDebug() << "Error: No se encontró el recibo actual.";
    return;
  }
  
  // Obtains the html content string of the receipt.
  QString receiptHtml = receipt->htmlContent();
  
  // Creates the printing document.
  QTextDocument document;
  
  // Creates a string to define and store the document content.
  QString receiptHtmlContent =
      "<html>\n"
      "<head>\n"
      "<style>\n"
      "body { font-family: Arial, sans-serif; margin: 0px; font-size: 11pt; }\n"
      ".header { font-size: 14pt; font-weight: bold; text-align: left; margin-bottom: 0px; }\n"
      ".item { font-size: 11pt; margin-bottom: 5px; }\n"
      ".total { font-size: 14pt; font-weight: bold; margin-top: 0px; text-align: right; }\n"
      "</style>\n"
      "</head>\n"
      "<body>\n"
      "<div style='border-top: 100px dashed black; width: 100%; margin: 10px 0;'></div>\n"
      + receiptHtml +
      "</body>\n"
      "</html>";
  
  // Sets the html document information.
  document.setHtml(receiptHtmlContent);
  
  // Sets the document margins to zero.
  document.setDocumentMargin(0);
  
  // Display the document information into the printer.
  document.print(printer);
}

void Pos::addProductToReceipt(const Product& product) {
  if (this->model.getPageAccess(2) == User::PageAccess::EDITABLE) {
    // Checks that there's al least 1 receipt openend to accept the click from a
    // product selection button.
    if (this->ui->openedReceiptsArea->layout()->count() > 0) {
      // Retrieve the currently active widget from the receiptStack  
      QWidget* currentWidget = this->receipStack->currentWidget();
      
      // Safely cast the QWidget to a Receipt pointer using qobject_cast
      Receipt* currentReceipt = qobject_cast<Receipt*>(currentWidget);
      
      // Checks that the current receipt pointer isn't null.
      if (currentReceipt) {
        // Adds the product into the current receipt.
        currentReceipt->addProduct(product);
      } else {
        qDebug() << "Error: The current widget is not a Receipt.";
      }
    } else {
      // Displays a warning message box to show the error to the user.
      QMessageBox::warning(this, "Error en la adición del producto"
          , "Por favor, primero cree una nueva factura.");
    }
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
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
    
    // Connects the button clicked signal with the pos slot ot handle it.
    this->connect(button
        , &ProductSelectionButton::on_productSelectionButton_clicked
        , this
        , &Pos::addProductToReceipt);
    
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
