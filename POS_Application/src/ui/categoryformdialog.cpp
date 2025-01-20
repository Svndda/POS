#include <vector>
#include <string>

#include "categoryformdialog.h"
#include "ui_categoryformdialog.h"

CategoryFormDialog::CategoryFormDialog(QWidget *parent
     , std::vector<std::string> categories
     , std::string category)
    : QDialog(parent)
    , ui(new Ui::CategoryFormDialog)
    , existingCategories(categories)
    , newCategory(category) {
  ui->setupUi(this);
  // Sets the category line edit text with the given string in the constructor.
  this->ui->categoryName_lineEdit->setText(this->newCategory.data());
  // Connects the slot of the accept button of the QDialog.
  this->connect(ui->acceptCategory_button
          , &QPushButton::clicked
          , this
          , &CategoryFormDialog::on_acceptCategory_button_clicked);
  // Connects the slot of the cancel button of the QDialog.  
  this->connect(ui->cancelCategory_button
          , &QPushButton::clicked
          , this
          , &CategoryFormDialog::on_cancelCategory_button_clicked);
}

CategoryFormDialog::~CategoryFormDialog() {
  delete ui;
}

std::string CategoryFormDialog::getNewCategory() {
  return this->newCategory;
}

void CategoryFormDialog::on_acceptCategory_button_clicked() {
  // Saves the current text in the category line edit as the new category
  // desire by the user.
  this->newCategory = this->ui->categoryName_lineEdit->text().toStdString();
  // Tries to find the new category in the existing categories vector.
  auto it = std::find(this->existingCategories.begin()
      , this->existingCategories.end(), this->newCategory);
  // Checks if the new category already exist, if not, then accept the dialog.
  if (it == this->existingCategories.end()) {
    // Stablish that the QDialog has finished correctly.
    this->accept();
  }
}

void CategoryFormDialog::on_cancelCategory_button_clicked() {
  // Cleans the new catgeory attribute to avoid to invoquin class to access
  // garbage data.
  this->newCategory = "";
  // Stablish that the QDialog has finished incorrectly.
  this->reject();
}
