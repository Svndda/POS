#ifndef PRODUCTSCATALOG_H
#define PRODUCTSCATALOG_H

#include "catalog.h"

namespace Ui {
class ProductsCatalog;
}

/**
 * @class ProductsCatalog
 * @brief Class for managing the products catalog in the application.
 *
 * This class inherits from Catalog and implements methods to display, add, edit, and delete products.
 * It provides a paginated view of the products available.
 */
class ProductsCatalog : public Catalog {
  Q_OBJECT

public:
  /**
   * @brief Constructs a ProductsCatalog object.
   * @param parent Pointer to the parent widget (default is nullptr).
   * @param appModel Reference to the POS_Model instance (default is POS_Model::getInstance()).
   */
  explicit ProductsCatalog(QWidget* parent = nullptr
      , POS_Model& appModel = POS_Model::getInstance());
  
  /**
   * @brief Destructor for the ProductsCatalog class.
   */
  ~ProductsCatalog();

private:
  Ui::ProductsCatalog* ui; ///< Pointer to the UI elements for ProductsCatalog.
  
protected:
  /**
   * @brief Refreshes the display with the products of the current page.
   * @param pageItems Number of products to display per page.
   */
  void refreshDisplay(const size_t pageItems) override;
  
  /**
   * @brief Sets up signal-slot connections for the UI components.
   */
  void setupConnections() override;
  
  // Private class functions.
private:
  /**
   * @brief Updates the UI to display the products on the current page.
   * @param visibleProducts Vector of pairs containing a category name and a Product object representing the visible products.
   * @param items Number of items to display.
   */
  void refreshProductDisplay(
      std::vector<std::pair<std::string, Product>> visibleProducts
      ,const size_t items);
  
  /**
   * @brief Deletes a product from the registered products.
   * @param index Index of the product to delete.
   */
  void deleteRegisteredProduct(size_t index);
  
  /**
   * @brief Opens the product editing dialog for a specific product.
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
