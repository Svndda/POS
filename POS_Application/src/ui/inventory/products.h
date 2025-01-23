// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <QWidget>
#include <vector>
#include <string>
#include "posmodel.h"
#include "product.h"
#include "inventory.h"

namespace Ui {
class Products;
}

/**
 * @class Products
 * @brief Handles the user interface and logic for managing products in the POS system.
 * 
 * This class extends the Inventory interface to display, add, edit, and delete 
 * products using a paginated UI. It uses a model (`POS_Model`) to interact with 
 * the underlying data.
 */
class Products : public Inventory {
  Q_OBJECT
  
private:
  Ui::Products* ui; ///< Pointer to the UI elements for the Products class.

public:
  /**
   * @brief Constructor for the Products class.
   * 
   * @param parent The parent QWidget (default is nullptr).
   * @param model Reference to the singleton POS_Model instance.
   */
  explicit Products(QWidget *parent = nullptr
      , POS_Model& model = POS_Model::getInstance());
  
  /**
   * @brief Destructor for the Products class.
   */
  ~Products();
  
protected:
  /**
   * @brief Refreshes the display with the products of the current page.
   * 
   * @param pageItems Number of items to display per page.
   */
  void refreshDisplay(const size_t pageItems) override;
  
  /**
   * @brief Sets up connections between UI elements and their respective slots.
   */
  void setupConnections() override;
  
  // Private class functions.
private:  
  /**
   * @brief Refreshes the UI to display products on the current page.
   * 
   * @param visibleProducts Vector of visible product-category pairs.
   * @param items Number of items to display.
   */
  void refreshProductDisplay(
      std::vector<std::pair<std::string, Product>> visibleProducts,
      const size_t items);  
  /**
   * @brief Deletes a product from the registered products.
   * 
   * @param index Index of the product to delete.
   */
  void deleteRegisteredProduct(size_t index);
  
  /**
   * @brief Opens the product editing dialog for a specific product.
   * 
   * @param index Index of the product to edit.
   */
  void editProductInformation(size_t index);
  
private slots:
  /**
   * @brief Slot triggered when the "Add Product" button is clicked.
   */
  void addProduct_button_clicked();

protected slots:
  /**
   * @brief Slot triggered when the "Next Page" button is clicked.
   */
  virtual void on_nextPage_button_clicked() override;
  
  /**
   * @brief Slot triggered when the "Previous Page" button is clicked.
   */
  virtual void on_previousPage_button_clicked() override;
  
  /**
   * @brief Slot triggered when the "Delete Product" button is clicked.
   */
  virtual void on_delete_button_clicked() override;
  
  /**
   * @brief Slot triggered when the "Edit Product" button is clicked.
   */
  virtual void on_edit_button_clicked() override;
  
  /**
   * @brief Slot triggered when the "Categories" button is clicked.
   */
  virtual void on_categories_button_clicked() override;
  
  /**
   * @brief Slot triggered when the "Supplies" button is clicked.
   */
  virtual void on_supplies_button_clicked() override;
  
  /**
   * @brief Slot triggered when the "Products" button is clicked.
   */
  virtual void on_products_button_clicked() override;
};

#endif // Products_H
