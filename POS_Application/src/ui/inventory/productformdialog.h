// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef PRODUCTFORMDIALOG_H
#define PRODUCTFORMDIALOG_H

#include <QDialog>
#include <map>
#include <string>
#include "product.h"

namespace Ui {
class ProductFormDialog;
}

/**
 * @class ProductFormDialog
 * @brief Dialog class for managing product details.
 * 
 * The `ProductFormDialog` class provides a user interface for entering and editing
 * product details such as name, ingredients, price, and category. It validates
 * input and updates product information based on user actions.
 */
class ProductFormDialog : public QDialog {
  Q_OBJECT
  
private:
  /**
   * @brief Pointer to the UI components of the dialog.
   */
  Ui::ProductFormDialog *ui;
  
  /**
   * @brief Reference to the map of registered products grouped by category.
   */
  std::map<std::string, std::vector<Product>>& registeredProducts;
  
  /**
   * @brief Product object containing the information entered or edited by the user.
   */
  Product createdProduct;
  
  /**
   * @brief The category of the product being created or edited.
   */
  QString productCategory;

public:
  /**
   * @brief Constructor for the ProductFormDialog class.
   * 
   * Initializes the dialog, populates the category combo box, and sets up connections
   * for handling user actions.
   * 
   * @param parent The parent widget of the dialog.
   * @param products A reference to the map of registered products grouped by category.
   * @param productToCreate The product object used to prepopulate the dialog fields.
   * @param productCategory The initial category for the product.
   */
  explicit ProductFormDialog(QWidget *parent,
                             std::map<std::string, std::vector<Product>>& products,
                             Product productToCreate,
                             QString productCategory);
  
  /**
   * @brief Destructor for the ProductFormDialog class.
   * 
   * Cleans up the resources allocated for the UI components.
   */
  ~ProductFormDialog();
  
  /**
   * @brief Formats a vector of ingredients into a readable string.
   * 
   * Converts a list of `SupplyItem` objects into a comma-separated string,
   * where each ingredient includes its name and quantity.
   * 
   * @param ingredients A vector of `SupplyItem` objects.
   * @return A formatted `QString` representation of the ingredients.
   */
  QString formatProductIngredients(const std::vector<SupplyItem>& ingredients);
  
  /**
   * @brief Retrieves the created or edited product.
   * 
   * @return A `Product` object with the details entered by the user.
   */
  Product getProduct();
  
  /**
   * @brief Retrieves the category of the product selected by the user.
   * 
   * @return A `QString` containing the product category.
   */
  QString getProductCategory();
  
  /**
   * @brief Populates the dialog fields with product information.
   * 
   * Fills in the UI elements with the product's name, ingredients, price, and
   * category based on the provided product object and category.
   * 
   * @param productToEdit The product object to use for populating fields.
   * @param productCategory The category to be selected in the combo box.
   */
  void setProductInfo(Product& productToEdit, QString productCategory);
private slots:
  /**
   * @brief Slot for handling the "Accept" button click event.
   * 
   * Validates the product details entered by the user and, if valid, creates
   * or updates the product. Displays a warning if the input is incorrect.
   */
  void on_acceptProduct_button_clicked();
  
  /**
   * @brief Slot for handling the "Cancel" button click event.
   * 
   * Closes the dialog without saving changes, rejecting the user's input.
   */
  void on_cancelProduct_button_clicked();
};

#endif // PRODUCTFORMDIALOG_H
