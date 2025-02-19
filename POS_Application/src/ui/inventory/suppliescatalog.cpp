// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include "suppliescatalog.h"
#include "ui_suppliescatalog.h"

#include <vector>
#include <string>
#include <QMessageBox>

#include "posmodel.h"
#include "util.h"
#include "supply.h"
#include "supplyformdialog.h"

SuppliesCatalog::SuppliesCatalog(QWidget *parent, POS_Model& model)
    : Catalog(parent, model)
    , ui(new Ui::SuppliesCatalog) {
  this->ui->setupUi(this);
  // Setups the ui elements conecctions with their function slots.
  this->setupConnections();
  // Refresh for the first time the ui information.
  this->refreshDisplay(this->itemsPerPage);
}

SuppliesCatalog::~SuppliesCatalog() {
  delete this->ui;
}

void SuppliesCatalog::refreshDisplay(const size_t pageItems) {
  this->refreshSuppliesDisplay(this->model.getSuppliesForPage(
      this->currentPageIndex, pageItems), pageItems);
}

void SuppliesCatalog::setupConnections() {
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
    this->connect(deleteButton , &QPushButton::clicked
        , this, &SuppliesCatalog::on_delete_button_clicked);
    this->connect(editButton , &QPushButton::clicked
        , this, &SuppliesCatalog::on_edit_button_clicked);
  }
  // Connects the funtions that handles the next and previous page of registered
  // categories.
  this->connect(this->ui->nextPage_button, &QPushButton::clicked
      , this, &SuppliesCatalog::on_nextPage_button_clicked);
  this->connect(this->ui->previousPage_button, &QPushButton::clicked
      , this, &SuppliesCatalog::on_previousPage_button_clicked);
  // Connects the function that handles the add category button.
  this->connect(this->ui->addSupply_button, &QPushButton::clicked
      , this, &SuppliesCatalog::addSupply_button_clicked);
}

void SuppliesCatalog::refreshSuppliesDisplay(
    const std::vector<Supply>& visibleSupplies
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
  
  const size_t offset = this->currentPageIndex * 9;
  // Generates the page label text tha indicates the indexes of the products
  // displayed in the actual page.
  const QString pageLabelText = QString("Mostrando suministros %1 hasta %2 de %3"
      " suministros").arg(offset + 1).arg(std::min(offset + 9
      , this->model.getNumberOfSupplies())).arg(
      this->model.getNumberOfSupplies());
  this->ui->pageDisplayingNumber_label->setText(pageLabelText);
}

void SuppliesCatalog::addSupply_button_clicked() {
  if (this->model.getPageAccess(2) == User::PageAccess::EDITABLE) {
    // Creates a dialog to manage the creation of a new supply.
    SupplyFormDialog dialog(this, this->model.getRegisteredSupplies()
        , Supply());
    
    // Executes the dialog and checks if were accepted.
    if (dialog.exec() == QDialog::Accepted) {
      // Obtain the supply created in the dialog.
      const Supply supply = dialog.getNewSupply();
      // Try to add the supply into the register of supplies.
      if (this->model.addSupply(supply)) {
        // Update the supplies display.
        this->refreshDisplay(this->itemsPerPage);
      } else {
        QMessageBox::information(this, "Informacion inválida"
            , "No se añadió el suministro.");  
      }
    } else {
      qDebug() << "Se cancelo la creacion de un producto";
    }
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void SuppliesCatalog::on_delete_button_clicked() {
  if (this->model.getPageAccess(2) == User::PageAccess::EDITABLE) {
    // Catch the pointer to the button object that sended the signal.
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    // If there's a pointer, then.
    if (button) {
      // Obtain the index of the display button.
      const size_t buttonIndex = button->property("index").toUInt();
      qDebug() << "Button clicked, index:" << buttonIndex;
      // Gets the supplies vector for the actual page.
      const auto suppliesForPage = this->model.getSuppliesForPage(
          this->currentPageIndex, this->itemsPerPage);
      // Checks that that the button index is lower than the supplies for this
      // page to avoid an empty row.
      if (buttonIndex < suppliesForPage.size()) {
        // Gets the row supply.
        Supply supply = suppliesForPage[buttonIndex];
        if (this->model.removeSupply(supply)) {
          this->refreshDisplay(this->itemsPerPage);
        } else {
          QMessageBox::warning(this, "Error"
              , "No se pudo eliminar el suministro.");
        }
      }
    }
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void SuppliesCatalog::on_edit_button_clicked() {
  if (this->model.getPageAccess(2) == User::PageAccess::EDITABLE) {
    // Catch the pointer to the button object that sended the signal.  
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    // Checks if the pointer is valid.
    if (button) {
      // Gets the index of the button.
      const size_t buttonIndex = button->property("index").toUInt();
      // Gets the supplies vector for the actual page.
      const auto suppliesForPage = this->model.getSuppliesForPage(
          this->currentPageIndex, this->itemsPerPage);
      // Checks that that the button index is lower than the supplies for this
      // page to avoid an empty row.
      if (buttonIndex < suppliesForPage.size()) {
        // Gets the row supply.
        const Supply oldSupply = suppliesForPage[buttonIndex];
        qDebug() << "Button clicked, index:" << buttonIndex << " "
                 << oldSupply.getName();
        // Creates a dialog to manage the existing supply editing.
        SupplyFormDialog dialog(this, this->model.getRegisteredSupplies()
            , oldSupply);
        
        if (dialog.exec() == QDialog::Accepted) {
          const Supply newSupply = dialog.getNewSupply();
          if (this->model.editSupply(oldSupply, newSupply)) {
            // Updates the display with the new supply.
            this->refreshDisplay(this->itemsPerPage);
            qDebug() << "Se ha modificado una categoria exitosamente";
          } else {
            QMessageBox::information(this, "Informacion inválida"
                , "No se editó el suministro.");
          }
        } else {
          qDebug() << "Se cancelo la edicion de una categoria";
        }
      }
    }
  } else {
    QMessageBox::information(this, "Acceso restrido."
        , "El usuario no posee los permisos de edición.");
  }
}

void SuppliesCatalog::on_nextPage_button_clicked() {
  // Calculates the supplies page start and end indexes for the next page.
  const size_t suppliesPageIt = (this->currentPageIndex + 1) * 9;
  const size_t suppliesPageIt2 = suppliesPageIt + 9;
  // Checks if the indexes the number of registered supplies is greather or
  // between the next page indexes.
  if (this->model.getNumberOfSupplies() >= suppliesPageIt) {
    ++this->currentPageIndex;
    // Updates the supplies information in the display to show the next supplies
    // page.
    this->refreshDisplay(this->itemsPerPage);
  }
  qDebug() << "Boton de avance: " << this->currentPageIndex;
}

void SuppliesCatalog::on_previousPage_button_clicked() {
  // Checks that the actual page is not the first one.
  if (this->currentPageIndex > 0) {
    // Decrements the page index.
    --this->currentPageIndex;
    // Updates the product information in the display to show the previous
    // product page.
    this->refreshDisplay(this->itemsPerPage);
  }
  std::cout << "Boton de retroceso: " << this->currentPageIndex << std::endl;
}
