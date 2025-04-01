#include "cashierpage.h"
#include "cashierdialog.h"
#include "expensedialog.h"
#include "incomelabel.h"
#include "ui_cashierpage.h"

#include <QString>
#include <QDatetime>
#include <QRegularexpression>
#include <QMessageBox>

CashierPage::CashierPage(QWidget *parent, Model& appmodel)
    : QWidget(parent)
    , ui(new Ui::CashierPage)
    , model(appmodel) {
  ui->setupUi(this);
  
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignTop);
  this->ui->cashierReceipts_content->setLayout(layout);
}

CashierPage::~CashierPage() {
  delete ui;
}

void CashierPage::on_openCashier_button_clicked() {
  if (!this->model.isCashierOpened()) {
    this->model.openCashier();
    this->ui->cashierInformationlabel->setText(
        QString("Caja abierta por: %1, a las %2.")
        .arg(this->model.getCurrentUser().getUsername().data())
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
    );
    this->ui->totalAmount_label->setText("Monto total de la caja: ₡30000.00");
    this->update();
  } else {
    // Displays a warning message box to show the error to the user.
    QMessageBox::warning(this, "Error en caja"
        , "Ya existe una caja abierta.");
  }
}

void CashierPage::on_closeCashier_button_clicked() {
  if (this->model.isCashierOpened()) {
    QLayout* layout = this->ui->cashierReceipts_content->layout();      
    if (layout->isEmpty()) {
      this->model.closeCashier();
      
      this->ui->cashierInformationlabel->setText(
          "No hay ninguna caja abierta.");
      this->ui->totalAmount_label->setText("Monto total de la caja: ₡00.00");
      this->update();
      return;
    }
    CashierDialog cashierDialog(this, this->incomes
        , this->expenses, this->model.getOngoingReceipts());
    
    if (cashierDialog.exec() == QDialog::Accepted) {
      this->model.closeCashier();
      
      this->ui->cashierInformationlabel->setText(
          "No hay ninguna caja abierta.");
            
      if (!layout) return;
      while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
          layout->removeWidget(widget);
          widget->hide();  // Opcional: Ocultar para que no quede visible
        }
        delete item;  // Solo elimina el item, pero no los widgets
      }
      
      this->ui->totalAmount_label->setText("Monto total de la caja: ₡00.00");
      this->update();
    } else {
      this->reloadCashierElements();
    }
  }
}

void CashierPage::reloadCashierElements() {
  QLayout* layout = this->ui->cashierReceipts_content->layout();
  for (const auto& income : this->incomes) {
    layout->addWidget(income);
  }
  for (const auto& expense : this->expenses) {
    layout->addWidget(expense);
  }
  layout->update();
  this->update();
}

void CashierPage::on_addExpense_button_clicked() {
  if (this->model.isCashierOpened()) {
    ExpenseDialog expenseDialog(this);
    
    this->connect(&expenseDialog, &ExpenseDialog::expenseCreated
        , this, &CashierPage::handleCreatedExpense);
    
    expenseDialog.exec();
  } else {
    // Displays a warning message box to show the error to the user.
    QMessageBox::warning(this, "Error en caja"
        , "Por favor, abra la caja antes de ingresar un gasto.");
  }
}

void CashierPage::handleCreatedExpense(const QString expenseName
    , const double expensePrice) {
  QLayout* layout = this->ui->cashierReceipts_content->layout();
  ExpenseLabel* expense = new ExpenseLabel(
      this, expenseName
      , QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
      , expensePrice
  );
  
  layout->addWidget(expense);
  
  this->ui->totalAmount_label->setText(
      QString("Monto total de la caja: %1")
          .arg(QString::number(this->obtainTotalAmount() - expensePrice))
  );
  this->update();
  this->expenses.emplace_back(expense);
}

void CashierPage::addProcessedReceipt() {
  QLayout* layout = this->ui->cashierReceipts_content->layout();  
  Receipt newReceipt = this->model.getOngoingReceipts().back();
  
  IncomeLabel* incomeLabel = new IncomeLabel(
      this
      , newReceipt.getID(), newReceipt.getDateTime()
      , newReceipt.getPaymentMethod(), newReceipt.getPrice()
  );
  
  layout->addWidget(incomeLabel);
  layout->update();
  
  if (/*newReceipt.getPaymentMethod() == "Efectivo"*/ 1) {
    this->ui->totalAmount_label->setText(
        QString("Monto total de la caja: %1")
            .arg(QString::number(this->obtainTotalAmount()
            + newReceipt.getPrice()))
    );
  }
  
  this->incomes.emplace_back(incomeLabel);
  this->update();
}

double CashierPage::obtainTotalAmount() {
  QString mixedStr = this->ui->totalAmount_label->text();
  QRegularExpression regex("(\\d+\\.?\\d*)");
  QRegularExpressionMatch match = regex.match(mixedStr);
  
  if (match.hasMatch()) {
    double extractedValue = match.captured(1).toDouble();
    qDebug() << "Número extraído:" << extractedValue;
    return extractedValue;
  }
  
  return 0;
}
