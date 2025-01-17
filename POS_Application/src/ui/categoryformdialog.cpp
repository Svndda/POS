#include "categoryformdialog.h"
#include "ui_categoryformdialog.h"

CategoryFormDialog::CategoryFormDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CategoryFormDialog) {
  ui->setupUi(this);
}

CategoryFormDialog::~CategoryFormDialog()
{
  delete ui;
}
