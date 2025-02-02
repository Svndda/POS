#ifndef PRODUCTSCATALOG_H
#define PRODUCTSCATALOG_H

#include "catalog.h"

namespace Ui {
class ProductsCatalog;
}

class ProductsCatalog : public Catalog
{
  Q_OBJECT

public:
  explicit ProductsCatalog(QWidget* parent = nullptr
      , POS_Model& appModel = POS_Model::getInstance());
  ~ProductsCatalog();

private:
  Ui::ProductsCatalog* ui;
  
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
};

#endif // PRODUCTSCATALOG_H
