#include "categoriespage.h"
#include "ui_categoriespage.h"

#include "util.h"

CategoriesPage::CategoriesPage(QWidget *parent, POS_Model model)
    : QWidget(parent)
    , ui(new Ui::CategoriesPage)
    , appModel(model) {
  ui->setupUi(this);
  
  QString deleteButtonName("");
  QString editButtonName("");
  for (size_t index = 0; index < 9; ++index) {
    editButtonName = QString("editProduct_button_%1").arg(index);
    deleteButtonName = QString("deleteProduct_button_%1").arg(index);
    QPushButton* deleteButton = this->findChild<QPushButton *>(deleteButtonName);
    QPushButton* editButton = this->findChild<QPushButton *>(editButtonName);
    connect(deleteButton , &QPushButton::clicked
        , this, &CategoriesPage::on_deleteProduct_button_clicked);
    connect(editButton , &QPushButton::clicked
        , this, &CategoriesPage::on_editProduct_button_clicked);
  }
  
  connect(this->ui->nextProductPage_button, &QPushButton::clicked
      , this, &CategoriesPage::on_nextProductPage_button_clicked);
  connect(this->ui->previousProductPage_button, &QPushButton::clicked
      , this, &CategoriesPage::on_previousProductPage_button_clicked);
  
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
    // Updates the label containing the number of products registered in the
    // category.
    Util::updatePrefixedLabel(this, "categorySize", labelIt, numberOfProducts);  
    // Increse the label iterator.
    ++labelIt;
  }
  
  size_t offset = this->categoryPageIndex * 9;
  // Generates the page label text tha indicates the indexes of the products
  // displayed in the actual page.
  QString pageLabelText = QString("Mostrando productos %1 hasta %2 de %3"
      " productos").arg(offset + 1).arg(offset + 9).arg(
      this->appModel.getNumberOfProducts());
  this->ui->pageProductsNumber_label->setText(pageLabelText);
}

void CategoriesPage::on_deleteProduct_button_clicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    size_t index = button->property("index").toUInt(); // Recuperar índice
    qDebug() << "Button clicked, index:" << index;
  }
}

void CategoriesPage::on_editProduct_button_clicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    size_t index = button->property("index").toUInt();
    qDebug() << "Button clicked, index:" << index;
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
