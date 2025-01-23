// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
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
  // Set the initial value for the category name input field
  this->ui->categoryName_lineEdit->setText(this->newCategory.data());
  
  // Connect the "Accept" button to its slot
  this->connect(ui->acceptCategory_button,
                &QPushButton::clicked,
                this,
                &CategoryFormDialog::on_acceptCategory_button_clicked);
  
  // Connect the "Cancel" button to its slot
  this->connect(ui->cancelCategory_button,
                &QPushButton::clicked,
                this,
                &CategoryFormDialog::on_cancelCategory_button_clicked);
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
  std::string category = this->ui->categoryName_lineEdit->text().toStdString();
  // Tries to find the new category in the existing categories vector.
  auto it = std::find(this->existingCategories.begin()
      , this->existingCategories.end(), category);
  // Checks if the new category already exist, if not, then accept the dialog.
  if (it == this->existingCategories.end()) {
    this->newCategory = category;
    // Stablish that the QDialog has finished correctly.
    this->accept();
  }
}

void CategoryFormDialog::on_cancelCategory_button_clicked() {
  // Stablish that the QDialog has finished incorrectly.
  this->reject();
}
