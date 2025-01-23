// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef SUPPLYFORMDIALOG_H
#define SUPPLYFORMDIALOG_H

#include <QDialog>
#include <vector>
#include "supplyitem.h"

namespace Ui {
class SupplyFormDialog;
}

/**
 * @class SupplyFormDialog
 * @brief A dialog for creating or editing a supply item.
 *
 * This class provides a user interface to define or modify the properties of
 * a supply item, including its name, quantity, and measurement unit.
 */
class SupplyFormDialog : public QDialog {
  Q_OBJECT
  
private:
  Ui::SupplyFormDialog* ui;                   ///< Pointer to the UI layout.
  std::vector<SupplyItem> existingSupplies;  ///< List of existing supplies for validation.
  SupplyItem newSupply;                      ///< Supply item being created or modified.

public:
  /**
   * @brief Constructs a SupplyFormDialog instance.
   * @param parent Pointer to the parent widget.
   * @param supplies List of existing supplies for validation.
   * @param supply Supply item to edit, or an empty one for creation.
   */
  explicit SupplyFormDialog(QWidget *parent
      , std::vector<SupplyItem> supplies
      , SupplyItem supply);
  
  /**
   * @brief Destroys the dialog and its resources.
   */
  ~SupplyFormDialog();
  
public:
  /**
   * @brief Retrieves the updated or newly created supply item.
   * @return The new or modified SupplyItem object.
   */
  SupplyItem getNewSupply();
  
private slots:
  /**
   * @brief Handles the "Accept" button click event.
   *
   * Collects user inputs, creates or updates the supply item, 
   * and closes the dialog with an accepted result.
   */
  void on_acceptCategory_button_clicked();
  
  /**
   * @brief Handles the "Cancel" button click event.
   *
   * Discards changes and closes the dialog with a rejected result.
   */
  void on_cancelCategory_button_clicked();
};

#endif // SUPPLYFORMDIALOG_H
