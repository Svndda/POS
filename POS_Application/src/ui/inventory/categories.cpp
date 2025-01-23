// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include "categories.h"
#include "ui_categories.h"
#include "categoryformdialog.h"

#include "util.h"

Categories::Categories(QWidget *parent, POS_Model& model)
    : Inventory(parent, model)
    , ui(new Ui::Categories) {
  this->ui->setupUi(this);
  
  this->setupConnections();
  // Updates the view for first time with the correponding categories.
  this->refreshDisplay(this->itemsPerPage);
}

void Categories::setupConnections() {
  // String variables that will contain the different delete and edit buttons
  // name for each registered category.
  QString deleteButtonName("");
  QString editButtonName("");
  // Go through the rows of the page connecting their deletes and edit buttons
  //  with the funtions that handles that event.
  for (size_t index = 0; index < 9; ++index) {
    // Parse and index the correponding buttons name.
    editButtonName = QString("editProduct_button_%1").arg(index);
    deleteButtonName = QString("deleteProduct_button_%1").arg(index);
    // Find the buttons and store their pointers.
    QPushButton* deleteButton = this->findChild<QPushButton *>(deleteButtonName);
    QPushButton* editButton = this->findChild<QPushButton *>(editButtonName);
    // Connects the buttons with their functions.
    this->connect(deleteButton , &QPushButton::clicked
        , this, &Categories::on_delete_button_clicked);
    this->connect(editButton , &QPushButton::clicked
        , this, &Categories::on_edit_button_clicked);
  }
  // Connects the funtions that handles the next and previous page of registered
  // categories.
  this->connect(this->ui->nextProductPage_button, &QPushButton::clicked
      , this, &Categories::on_nextPage_button_clicked);
  this->connect(this->ui->previousProductPage_button, &QPushButton::clicked
      , this, &Categories::on_previousPage_button_clicked);
  // Connects the function that handles the button to change to the products
  // page.
  this->connect(this->ui->products_button, &QPushButton::clicked
      , this, &Categories::on_products_button_clicked);
  this->connect(this->ui->supplies_button, &QPushButton::clicked
      , this, &Categories::on_supplies_button_clicked);
  // Connects the function that handles the add category button.
  this->connect(this->ui->addCategory_button, &QPushButton::clicked
      , this, &Categories::addCategory_button_clicked);
}

void Categories::refreshDisplay(const size_t pageItems) {
  this->refreshCategoriesDisplay(
      this->model.getCategoriesForPage(this->currentPageIndex, pageItems)
      , pageItems);
}

void Categories::refreshCategoriesDisplay(
    std::vector<std::string> visibleCategories
    , const size_t items) {
  // Initiazates the label index iterator.
  size_t labelIt = 0;
  // Tranverse the different rows of products information.
  for (size_t index = 0; index < items; index++) {
    // Place holder labels text.
    QString category("-----");
    QString numberOfProducts("------");
    
    // If theres remaining products information to be displayed.
    if (index < visibleCategories.size()) {
      // Extracts all the product information to display it on the screen.
      category = visibleCategories[index].data();
      numberOfProducts = QString::number(this->model.getSizeOfCategory(
          visibleCategories[index]));
    }
    // Updates the the different labels for each product information that is
    // displayed in the UI.
    // Updates the product text contained in the categoryName of the matching
    // label.
    Util::updatePrefixedLabel(this, "categoryName", labelIt, category);
    // Updates the label containing the number of categories registered in the
    // system.
    Util::updatePrefixedLabel(this, "categorySize", labelIt, numberOfProducts);  
    // Increse the label iterator.
    ++labelIt;
  }
  
  size_t offset = this->currentPageIndex * 9;
  // Generates the page label text tha indicates the indexes of the products
  // displayed in the actual page.
  QString pageLabelText = QString("Mostrando categorias %1 hasta %2 de %3"
      " categorías").arg(offset + 1).arg(std::min(offset + 9,
      this->model.getNumberOfCategories())).arg(
      this->model.getNumberOfCategories());
  this->ui->pageProductsNumber_label->setText(pageLabelText);
}

void Categories::on_products_button_clicked() {
  // Emit the signal for the app controller to handle/receive it.
  emit this->products_button_signal();
}

void Categories::on_supplies_button_clicked() {
  qDebug() << "Supplies button clicked"; // Verificar si la función se ejecuta.
  
  // Emit the signal for the app controller to handle/receive it.
  emit this->supplies_button_signal();
}

void Categories::on_categories_button_clicked() {
  
}

void Categories::addCategory_button_clicked() {
  // Creates a dialog to manage the category creation.
  CategoryFormDialog dialog(this, this->model.getRegisteredCategories()
      , std::string());
  
  // Execute the dialog with empty information.
  if (dialog.exec() == QDialog::Accepted) {
    qDebug() << "Se acepto el dialogo y se agrego una nueva categoría";
    // Obtain and adds the created category by the user on the dialog.
    this->model.addCategory(dialog.getNewCategory());
    // Refresh the categories display.
    this->refreshDisplay(this->itemsPerPage);
  } else {
    qDebug() << "Se cancelo la creacion de una categoria";
  }
}

void Categories::on_delete_button_clicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    // Search for the property index in the button to see their index.
    size_t buttonIndex = button->property("index").toUInt();
    qDebug() << "Button clicked, index:" << buttonIndex;
    // Gets the categories vector for the actual page.
    auto categoriesForPage = this->model.getCategoriesForPage(
        this->currentPageIndex, this->itemsPerPage);
    // Checks that that the button index is lower than the categories for this
    // page to avoid an empty row.
    if (buttonIndex < categoriesForPage.size()) {
      // Gets the row category.
      std::string category = categoriesForPage[buttonIndex];
      // Removes the category from the registered ones.
      this->model.removeCategory(category);
      // Refresh the categories display with the updated data.
      this->refreshDisplay(this->itemsPerPage);
    }
  }
}

void Categories::on_edit_button_clicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  // Checks if the pointer is valid.
  if (button) {
    // Search for the property index in the button to see their index.
    size_t buttonIndex = button->property("index").toUInt();
    // Gets the categories vector for the actual page.
    auto categoriesForPage = this->model.getCategoriesForPage(
        this->currentPageIndex, this->itemsPerPage);
    // Checks that that the button index is lower than the categories for this
    // page to avoid an empty row.
    if (buttonIndex < categoriesForPage.size()) {
      // Gets the row category.
      std::string oldCategory = this->model.getCategoriesForPage(
          this->currentPageIndex, this->itemsPerPage)[buttonIndex];
      qDebug() << "Button clicked, index:" << buttonIndex << " " << oldCategory;
      // Creates a dialog to manage the existing category editing.
      CategoryFormDialog dialog(this, this->model.getRegisteredCategories()
          , oldCategory);
      // Executes the dialog to manage the category creation.
      if (dialog.exec() == QDialog::Accepted) {
        qDebug() << "Se ha modificado una categoria exitosamente";
        std::string newCategory = dialog.getNewCategory();
        // Updates the category name to the name wiven by the user.
        this->model.editCategory(oldCategory, newCategory);
        // Updates the display with the new category.
        this->refreshDisplay(this->itemsPerPage);
      } else {
        qDebug() << "Se cancelo la edicion de una categoria";
      } 
    }
  }
}

void Categories::on_nextPage_button_clicked() {
  // Calculates the category page start and end indexes for the next page.
  size_t categoryPageIt = (this->currentPageIndex + 1) * 9;
  size_t categoryPageIt2 = categoryPageIt + 9;
  // Checks if the indexes the number of registered categories is greather or
  // between the next page indexes.
  if (this->model.getNumberOfCategories() >= categoryPageIt
      && this->model.getNumberOfCategories() <= categoryPageIt2) {
    // Increments the current page.
    ++this->currentPageIndex;
    // Updates the information in the display to show the next categories
    // page.
    this->refreshDisplay(this->itemsPerPage);
  }
  qDebug() << "Boton de avance: " << this->currentPageIndex;
}

void Categories::on_previousPage_button_clicked() {
  // Checks that the actual page is not the first one.
  if (this->currentPageIndex > 0) {
    // Decrements the page index.
    --this->currentPageIndex;
    // Updates the information in the display to show the previous
    // categories page.
    this->refreshDisplay(this->itemsPerPage);
  }
  std::cout << "Boton de retroceso: " << this->currentPageIndex << std::endl;
}

Categories::~Categories() {
  delete ui;
}
