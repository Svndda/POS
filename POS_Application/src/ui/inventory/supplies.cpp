// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include "supplies.h"
#include "ui_supplies.h"

#include <vector>
#include <string>
#include <supplyitem.h>

#include "posmodel.h"
#include "util.h"
#include "supplyformdialog.h"

Supplies::Supplies(QWidget *parent, POS_Model& model)
    : Inventory(parent, model)
    , ui(new Ui::Supplies) {
  ui->setupUi(this);
  this->setupConnections();
  this->refreshDisplay(this->itemsPerPage);
}

Supplies::~Supplies() {
  delete ui;
}

void Supplies::refreshDisplay(const size_t pageItems) {
  this->refreshSuppliesDisplay(this->model.getSuppliesForPage(
      this->currentPageIndex, pageItems), pageItems);
}

void Supplies::setupConnections() {
  // String variables that will contain the different delete and edit buttons
  // name for each registered category.
  QString deleteButtonName("");
  QString editButtonName("");
  // Go through the rows of the page connecting their deletes and edit buttons
  //  with the funtions that handles that event.
  for (size_t index = 0; index < this->itemsPerPage; ++index) {
    // Parse and index the correponding buttons name.
    editButtonName = QString("editProduct_button_%1").arg(index);
    deleteButtonName = QString("deleteProduct_button_%1").arg(index);
    // Find the buttons and store their pointers.
    QPushButton* deleteButton = this->findChild<QPushButton *>(deleteButtonName);
    QPushButton* editButton = this->findChild<QPushButton *>(editButtonName);
    // Connects the buttons with their functions.
    connect(deleteButton , &QPushButton::clicked
            , this, &Supplies::on_delete_button_clicked);
    connect(editButton , &QPushButton::clicked
            , this, &Supplies::on_edit_button_clicked);
  }
  // Connects the funtions that handles the next and previous page of registered
  // categories.
  connect(this->ui->nextProductPage_button, &QPushButton::clicked
          , this, &Supplies::on_nextPage_button_clicked);
  connect(this->ui->previousProductPage_button, &QPushButton::clicked
          , this, &Supplies::on_previousPage_button_clicked);
  // Connects the function that handles the button to change to the products
  // page.
  connect(this->ui->products_button, &QPushButton::clicked
          , this, &Supplies::on_products_button_clicked);
  connect(this->ui->categories_button, &QPushButton::clicked
          , this, &Supplies::on_supplies_button_clicked);
  // Connects the function that handles the add category button.
  connect(this->ui->addCategory_button, &QPushButton::clicked
          , this, &Supplies::addSupply_button_clicked);
}

void Supplies::refreshSuppliesDisplay(
    const std::vector<SupplyItem>& visibleSupplies
    , const size_t items) {
  // Initiazates the label index iterator.
  size_t labelIt = 0;
  // Tranverse the different rows of products information.
  for (size_t index = 0; index < items; index++) {
    // Place holder labels text.
    QString name("-----");
    QString quantity("------");
    QString measureUnit("------");
    
    // If theres remaining products information to be displayed.
    if (index < visibleSupplies.size()) {
      // Extracts all the product information to display it on the screen.
      name = visibleSupplies[index].getName().data();
      quantity = QString::number(visibleSupplies[index].getQuantity());
      measureUnit = visibleSupplies[index].getMeasure().data();
    }
    // Updates the the different labels for each product information that is
    // displayed in the UI.
    // Updates the product text contained in the categoryName of the matching
    // label.
    Util::updatePrefixedLabel(this, "supplyName", labelIt, name);
    // Updates the label containing the number of categories registered in the
    // system.
    Util::updatePrefixedLabel(this, "supplyQuantity", labelIt, quantity);
    
    Util::updatePrefixedLabel(this, "measureUnit", labelIt, measureUnit);  
    // Increse the label iterator.
    ++labelIt;
  }
  
  size_t offset = this->currentPageIndex * 9;
  // Generates the page label text tha indicates the indexes of the products
  // displayed in the actual page.
  QString pageLabelText = QString("Mostrando suministros %1 hasta %2 de %3"
      " suministros").arg(offset + 1).arg(std::min(offset + 9
      , this->model.getNumberOfSupplies())).arg(
      this->model.getNumberOfSupplies());
  this->ui->pageProductsNumber_label->setText(pageLabelText);
}

void Supplies::addSupply_button_clicked() {
  SupplyFormDialog dialog(this
      , this->model.getRegisteredSupplies()
      , SupplyItem());
  
  if (dialog.exec() == QDialog::Accepted) {
    SupplyItem supply = dialog.getNewSupply();
    if (this->model.addSupply(supply)) {
      this->refreshDisplay(this->itemsPerPage);
    } else {
      qDebug() << "No se ha agregado el suministro"
          ", ya existe uno con ese nombre.";      
    }
  } else {
    qDebug() << "Se cancelo la creacion de un producto";
  }
}

void Supplies::on_nextPage_button_clicked() {
  
}

void Supplies::on_previousPage_button_clicked() {
  
}

void Supplies::on_delete_button_clicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    size_t buttonIndex = button->property("index").toUInt();
    qDebug() << "Button clicked, index:" << buttonIndex;
    // Gets the supplies vector for the actual page.
    auto suppliesForPage = this->model.getSuppliesForPage(
        this->currentPageIndex, this->itemsPerPage);
    // Checks that that the button index is lower than the supplies for this
    // page to avoid an empty row.
    if (buttonIndex < suppliesForPage.size()) {
      // Gets the row supply.
      SupplyItem supply = suppliesForPage[buttonIndex];
      if (this->model.removeSupply(supply)) {
        this->refreshDisplay(this->itemsPerPage);        
      }
    }
  }
}

void Supplies::on_edit_button_clicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  // Checks if the pointer is valid.
  if (button) {
    // Gets the index of the button.
    size_t buttonIndex = button->property("index").toUInt();
    // Gets the supplies vector for the actual page.
    auto suppliesForPage = this->model.getSuppliesForPage(
        this->currentPageIndex, this->itemsPerPage);
    // Checks that that the button index is lower than the supplies for this
    // page to avoid an empty row.
    if (buttonIndex < suppliesForPage.size()) {
      // Gets the row supply.
      SupplyItem oldSupply = this->model.getSuppliesForPage(
          this->currentPageIndex, this->itemsPerPage)[buttonIndex];
      qDebug() << "Button clicked, index:" << buttonIndex << " " << oldSupply.getName();
      // Creates a dialog to manage the existing supply editing.
      SupplyFormDialog dialog(this
          , this->model.getRegisteredSupplies()
          , oldSupply);
      
      if (dialog.exec() == QDialog::Accepted) {
        SupplyItem newSupply = dialog.getNewSupply();
        if (this->model.editSupply(oldSupply, newSupply)) {
          // Updates the display with the new supply.
          this->refreshDisplay(this->itemsPerPage);
          qDebug() << "Se ha modificado una categoria exitosamente";
        } else {
          qDebug() << "Se no ha modificado el suministro.";          
        }
      } else {
        qDebug() << "Se cancelo la edicion de una categoria";
      } 
    }
  }
}

void Supplies::on_categories_button_clicked() {
  // Emit the signal for the app controller to handle/receive it.  
  emit this->categories_button_signal();
}

void Supplies::on_supplies_button_clicked() {
  
}

void Supplies::on_products_button_clicked() {
  // Emit the signal for the app controller to handle/receive it.  
  emit this->products_button_signal();
}


