#include "categoriespage.h"
#include "ui_categoriespage.h"
#include "categoryformdialog.h"

#include "util.h"

CategoriesPage::
    CategoriesPage(QWidget *parent, POS_Model& model)
    : QWidget(parent)
    , ui(new Ui::CategoriesPage)
    , appModel(model) {
  ui->setupUi(this);
  
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
    connect(deleteButton , &QPushButton::clicked
        , this, &CategoriesPage::on_deleteProduct_button_clicked);
    connect(editButton , &QPushButton::clicked
        , this, &CategoriesPage::on_editProduct_button_clicked);
  }
  // Connects the funtions that handles the next and previous page of registered
  // categories.
  connect(this->ui->nextProductPage_button, &QPushButton::clicked
      , this, &CategoriesPage::on_nextProductPage_button_clicked);
  connect(this->ui->previousProductPage_button, &QPushButton::clicked
      , this, &CategoriesPage::on_previousProductPage_button_clicked);
  // Connects the function that handles the button to change to the products
  // page.
  connect(this->ui->products_button, &QPushButton::clicked
          , this, &CategoriesPage::on_products_button_clicked);
  // Connects the function that handles the add category button.
  connect(this->ui->addCategory_button, &QPushButton::clicked
          , this, &CategoriesPage::addCategory_button_clicked);
  
  // Updates the view for first time with the correponding categories.
  this->refreshCategoriesDisplay(this->appModel.getCategoriesForPage(
      this->categoryPageIndex));
}

void CategoriesPage::refreshCategoriesDisplay(
    std::vector<std::string> visibleCategories) {
  // Initiazates the label index iterator.
  size_t labelIt = 0;
  // Tranverse the different rows of products information.
  for (size_t index = 0; index < 9; index++) {
    // Place holder labels text.
    QString category("-----");
    QString numberOfProducts("------");
    
    // If theres remaining products information to be displayed.
    if (index < visibleCategories.size()) {
      // Extracts all the product information to display it on the screen.
      category = visibleCategories[index].data();
      numberOfProducts = QString::number(this->appModel.getSizeOfCategory(
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
  
  size_t offset = this->categoryPageIndex * 9;
  // Generates the page label text tha indicates the indexes of the products
  // displayed in the actual page.
  QString pageLabelText = QString("Mostrando categorias %1 hasta %2 de %3"
      " categorías").arg(offset + 1).arg(offset + 9).arg(
      this->appModel.getNumberOfCategories());
  this->ui->pageProductsNumber_label->setText(pageLabelText);
}

void CategoriesPage::on_products_button_clicked() {
  // Emit the signal for the app controller to handle/receive it.
  emit this->products_button_signal();
}

void CategoriesPage::addCategory_button_clicked() {
  CategoryFormDialog dialog(this, this->appModel.getRegisteredCategories()
      , std::string());
  
  if (dialog.exec() == QDialog::Accepted) {
    qDebug() << "Se acepto el dialogo y se agrego una nueva categoría";
    this->appModel.addCategory(dialog.getNewCategory());
    this->refreshCategoriesDisplay(
        this->appModel.getCategoriesForPage(this->categoryPageIndex));
  } else {
    qDebug() << "Se cancelo la creacion de una categoria";
  }
}

void CategoriesPage::on_deleteProduct_button_clicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    size_t buttonIndex = button->property("index").toUInt();
    qDebug() << "Button clicked, index:" << buttonIndex;
    // Gets the categories vector for the actual page.
    auto categoriesForPage = this->appModel.getCategoriesForPage(
        this->categoryPageIndex);
    // Checks that that the button index is lower than the categories for this
    // page to avoid an empty row.
    if (buttonIndex < categoriesForPage.size()) {
      // Gets the row category.
      std::string category = this->appModel.getCategoriesForPage(
          this->categoryPageIndex)[buttonIndex];
      this->appModel.removeCategory(category);
      this->refreshCategoriesDisplay(
          this->appModel.getCategoriesForPage(this->categoryPageIndex));
    }
  }
}

void CategoriesPage::on_editProduct_button_clicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  // Checks if the pointer is valid.
  if (button) {
    // Gets the index of the button.
    size_t buttonIndex = button->property("index").toUInt();
    // Gets the categories vector for the actual page.
    auto categoriesForPage = this->appModel.getCategoriesForPage(
        this->categoryPageIndex);
    // Checks that that the button index is lower than the categories for this
    // page to avoid an empty row.
    if (buttonIndex < categoriesForPage.size()) {
      // Gets the row category.
      std::string oldCategory = this->appModel.getCategoriesForPage(
          this->categoryPageIndex)[buttonIndex];
      qDebug() << "Button clicked, index:" << buttonIndex << " " << oldCategory;
      // Creates a dialog to manage the existing category editing.
      CategoryFormDialog dialog(this, this->appModel.getRegisteredCategories()
          , oldCategory);
      
      if (dialog.exec() == QDialog::Accepted) {
        qDebug() << "Se ha modificado una categoria exitosamente";
        std::string newCategory = dialog.getNewCategory();
        this->appModel.editCategory(oldCategory, newCategory);
        // Updates the display with the new category.
        this->refreshCategoriesDisplay(
            this->appModel.getCategoriesForPage(this->categoryPageIndex));
      } else {
        qDebug() << "Se cancelo la edicion de una categoria";
      } 
    }
  }
}

void CategoriesPage::on_nextProductPage_button_clicked() {
  size_t productPageIt = (this->categoryPageIndex + 1) * 9;
  size_t productPageIt2 = productPageIt + 9;
  if (this->appModel.getNumberOfCategories() >= productPageIt
      && this->appModel.getNumberOfCategories() <= productPageIt2) {
    ++this->categoryPageIndex;
    // Updates the information in the display to show the next categories
    // page.
    this->refreshCategoriesDisplay(
        this->appModel.getCategoriesForPage(this->categoryPageIndex));
  }
  qDebug() << "Boton de avance: " << this->categoryPageIndex;
}

void CategoriesPage::on_previousProductPage_button_clicked() {
  // Checks that the actual page is not the first one.
  if (this->categoryPageIndex > 0) {
    // Decrements the page index.
    --this->categoryPageIndex;
    // Updates the information in the display to show the previous
    // categories page.
    this->refreshCategoriesDisplay(
        this->appModel.getCategoriesForPage(this->categoryPageIndex));
  }
  std::cout << "Boton de retroceso: " << this->categoryPageIndex << std::endl;
}

CategoriesPage::~CategoriesPage() {
  delete ui;
}
