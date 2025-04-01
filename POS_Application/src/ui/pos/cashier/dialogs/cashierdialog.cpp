#include "cashierdialog.h"
#include "ui_cashierdialog.h"

#include <QPrintdialog>
#include <QPrinterinfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>

#include "expenselabel.h"
#include "incomelabel.h"
#include "util.h"

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
  auto printer = Util::systemPrinter(this, "POS-80C");

  QPainter painter(printer.get());
  if (!painter.isActive()) {
    qDebug() << "Error: No se pudo iniciar el pintor.";
    return;
  }
  
  int printableWidth = printer->pageRect(QPrinter::DevicePixel).width();
  int margin = 5;
  int contentWidth = printableWidth - (2 * margin);
  
  QFont fontNormal("Courier", 10);
  QFont fontBold("Courier", 10, QFont::Bold);
  painter.setFont(fontNormal);
  
  QFontMetrics metrics(fontNormal);
  int lineHeight = metrics.height();  
  int y = 20;
  
  // Prints out the cashier information.
  Util::printWrappedLine(
      painter, "Cierre de caja", y, true
      , metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
      , y, true, metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, "===================", y, true
      , metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, this->ui->cash_label->text(), y, true
      , metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, this->ui->sinpe_label->text(), y, true
      , metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, this->ui->card_label->text(), y, true
      , metrics, margin, contentWidth, lineHeight
  );
  Util::printWrappedLine(
      painter, "===================", y, true
      , metrics, margin, contentWidth, lineHeight
  );
  
  Util::printWrappedLine(
      painter, this->ui->total_label->text(), y, true
      , metrics, margin, contentWidth, lineHeight
  );
}
