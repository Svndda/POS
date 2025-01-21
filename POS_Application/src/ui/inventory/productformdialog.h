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
 * @brief A dialog for displaying and editing product details.
 * 
 * This dialog allows the user to enter or edit the details of a product, such as 
 * its name, ingredients, price, and category. The dialog also validates the input
 * before creating or updating the product.
 */
class ProductFormDialog : public QDialog {
  Q_OBJECT
  
private:
  // Pointer to the ui object.
  Ui::ProductFormDialog *ui;
  // Map that contains all the registered drinks based on their category.
  std::map<std::string, std::vector<Product>>& registeredProducts;
  // Object that contains all the product information.
  Product createdProduct = Product();
  // String that contains the product's category.
  QString productCategory = QString();

public:
  /**
   * @brief Constructor for the ProductFormDialog class.
   * 
   * Initializes the dialog, populates the product category combo box with available categories 
   * from drinks and dishes, and sets up connections for the accept and cancel buttons.
   * 
   * @param parent Parent widget for the dialog.
   * @param drinks A map of drinks categories and corresponding products.
   * @param dishes A map of dishes categories and corresponding products.
   * @param product A Product object containing information to pre-fill the dialog fields.
   * @param category The product category to be selected initially in the combo box.
   */
  explicit ProductFormDialog(QWidget *parent
      , std::map<std::string, std::vector<Product>>& products
      , Product productToCreate
      , QString productCategory);
  
  /**
   * @brief Destructor for the ProductFormDialog class.
   * 
   * Cleans up the UI resources when the dialog is destroyed.
   */
  ~ProductFormDialog();
  
  /**
   * @brief Formats a list of product ingredients into a string.
   * 
   * Iterates through a vector of SupplyItem objects and generates a formatted string 
   * suitable for display in the product form.
   * 
   * @param ingredients A vector of SupplyItem objects to be formatted.
   * @return A QString containing the formatted product ingredients.
   */
  QString formatProductIngredients(const std::vector<SupplyItem>& ingredients);
  
  /**
   * @brief Retrieves the created or edited product.
   * 
   * @return A Product object containing the information entered or edited by the user.
   */
  Product getProduct();
  
  /**
   * @brief Retrieves the created or edited product.
   * 
   * @return A Product object containing the information entered or edited by the user.
   */
  QString getProductCategory();
  
  /**
   * @brief Sets the product information in the dialog fields.
   * 
   * Populates the product category, name, ingredients, and price into the respective UI elements.
   * 
   * @param productToEdit The product whose information will be displayed.
   * @param productCategory The product category to be set in the combo box.
   */
  void setProductInfo(Product& productToEdit, QString productCategory);
private slots:
  /**
   * @brief Slot connected to the accept button. Validates user input and saves the product information.
   * 
   * Validates the product name and ingredients entered by the user, ensuring that ingredients match the expected format.
   * If valid, it creates a new product and accepts the dialog. Otherwise, it displays a warning message box.
   */
  void on_acceptProduct_button_clicked();
  
  /**
   * @brief Slot connected to the cancel button. Closes the dialog without saving.
   * 
   * Rejects the dialog, signaling that the user canceled the operation.
   */
  void on_cancelProduct_button_clicked();
};

#endif // PRODUCTFORMDIALOG_H
