// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef CATEGORYFORMDIALOG_H
#define CATEGORYFORMDIALOG_H

#include <QDialog>
#include <QString>
#include <vector>
#include <string>

namespace Ui {
class CategoryFormDialog;
}

/**
 * @class CategoryFormDialog
 * @brief A dialog class that allows the user to create or edit categories in the application.
 * 
 * The `CategoryFormDialog` class provides an interface for users to input or modify category names.
 * It validates user input to ensure that the new category does not already exist in the list of 
 * existing categories. The dialog uses Qt signals and slots to handle user actions.
 */
class CategoryFormDialog : public QDialog {
  Q_OBJECT
  
private:
  /**
   * @brief Pointer to the UI components of the dialog.
   */
  Ui::CategoryFormDialog* ui;
  
  /**
   * @brief A vector containing the names of existing categories.
   * Used to validate the uniqueness of the new category.
   */
  std::vector<std::string> existingCategories;
  
  /**
   * @brief Stores the name of the new category entered by the user.
   */
  std::string newCategory;

public:
  /**
   * @brief Constructs a CategoryFormDialog object.
   * @param parent The parent widget of the dialog, defaulting to nullptr.
   * @param categories A vector of existing category names.
   * @param category The initial category name to prepopulate the input field.
   */
  explicit CategoryFormDialog(QWidget* parent,
                              std::vector<std::string> categories,
                              std::string category);
  
  /**
   * @brief Destructor to clean up allocated resources.
   */
  ~CategoryFormDialog();
  
public:
  /**
   * @brief Retrieves the new category name entered by the user.
   * @return The new category name as a `std::string`.
   */
  std::string getNewCategory();
  
private slots:
  /**
   * @brief Slot to handle the "Accept" button click event.
   * 
   * This method saves the user-inputted category name, validates it
   * against the list of existing categories, and accepts the dialog
   * if the new category is valid.
   */
  void on_acceptCategory_button_clicked();
  
  /**
   * @brief Slot to handle the "Cancel" button click event.
   * 
   * This method clears the new category name and rejects the dialog,
   * signaling that no changes should be made.
   */
  void on_cancelCategory_button_clicked();
  
};

#endif // CATEGORYFORMDIALOG_H
