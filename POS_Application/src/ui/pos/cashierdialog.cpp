#include "cashierdialog.h"
#include "ui_cashierdialog.h"

#include <QPrintdialog>
#include <QPrinterinfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>

#include "expenselabel.h"
#include "incomelabel.h"

CashierDialog::CashierDialog(QWidget *parent
    , const QVector<IncomeLabel*>& cashierIncomes
    , const QVector<ExpenseLabel*>& cashierExpenses
    , const std::vector<Receipt> &cashierReceipts)
    : QDialog(parent)
    , ui(new Ui::CashierDialog)
    , incomes(cashierIncomes)
    , expenses(cashierExpenses)
    , receipts(cashierReceipts){
  ui->setupUi(this);
  
  this->connect(this->ui->accept_button, &QPushButton::clicked
      , this, [this](){
    this->printCashierClosing();
    this->accept();});
  
  this->connect(this->ui->cancel_button, &QPushButton::clicked
      , this, [this](){this->reject();});
  
  QVBoxLayout* receiptsLayout = new QVBoxLayout;
  receiptsLayout->setAlignment(Qt::AlignTop);
  this->ui->ReceiptsAreaWidgetContents->setLayout(receiptsLayout);
  
  for (const auto& element : this->incomes) {
    receiptsLayout->addWidget(element);
    receiptsLayout->update();
  }
  
  QVBoxLayout* expensesLayout = new QVBoxLayout;
  expensesLayout->setAlignment(Qt::AlignTop);
  this->ui->ExpensesAreaWidgetContents->setLayout(expensesLayout);
  
  double expensesCost = 0;
  for (const auto& expense : this->expenses) {
    expensesCost += expense->getAmount();
    expensesLayout->addWidget(expense);
    expensesLayout->update();
  }
  
  std::vector<double> amounts(3, 0);
  for (const auto receipt : this->receipts) {
    if (receipt.getPaymentMethod() == "Efectivo") {
      amounts[0] += receipt.getPrice();
    } else if (receipt.getPaymentMethod() == "Sinpe") {
      amounts[1] += receipt.getPrice();
    } else if (receipt.getPaymentMethod() == "Tarjeta") {
      amounts[2] += receipt.getPrice();
    }
  }
  amounts[0] += 30000 - expensesCost;
  
  this->ui->cash_label->setText("Efectivo: ₡" + QString::number(amounts[0], 'f', 2));
  this->ui->sinpe_label->setText("Sinpe: ₡" + QString::number(amounts[1], 'f', 2));
  this->ui->card_label->setText("Tarjeta: ₡" + QString::number(amounts[2], 'f', 2));
  
  const double total = amounts[0] + amounts[1] + amounts[2];
  this->ui->total_label->setText("Total: ₡" + QString::number(total, 'f', 2));
  
  this->update();
}

CashierDialog::~CashierDialog() {
  delete ui;
}

void CashierDialog::printCashierClosing() {
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
    QMessageBox::critical(this, "Error"
        , "No se encontró una impresora válida.");
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
  printer.setPageSize(QPageSize(QSizeF(48, 60), QPageSize::Millimeter));
  printer.setPageMargins(QMarginsF(0, 0, 0, 0), QPageLayout::Millimeter);
  printer.setFullPage(true);
  
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
  
  int y = 20;
  printWrappedLine("Cierre de caja", y, true);
  y += 10;
  printWrappedLine(
      QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"), y);
  printWrappedLine("===================", y);
  
  printWrappedLine(this->ui->cash_label->text(), y);
  printWrappedLine(this->ui->sinpe_label->text(), y);
  printWrappedLine(this->ui->card_label->text(), y);
  printWrappedLine("===================", y);
  
  printWrappedLine(this->ui->total_label->text(), y, true);
}
