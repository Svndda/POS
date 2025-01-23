// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef CATEGORIES_H
#define CATEGORIES_H

#include "inventory.h"

namespace Ui {
class Categories;
}


/**
 * @class Categories
 * @brief A class that extends Inventory to handle categories management within the UI.
 */
class Categories : public Inventory {
  Q_OBJECT
  
private:
  /**
   * @brief Pointer to the UI object that manages the graphical interface.
   */
  Ui::Categories* ui;
  
public:
  /**
   * @brief Constructs a Categories object.
   * @param parent The parent widget, defaulting to nullptr.
   * @param appModel A reference to the POS_Model instance used for managing application data.
   */
  explicit Categories(QWidget *parent = nullptr
      , POS_Model& appModel = POS_Model::getInstance());
  /**
   * @brief Destructor that cleans up allocated resources.
   */
  ~Categories();
  
protected:
  /**
   * @brief Refreshes the category display with the current page of items.
   * @param pageItems The number of items to display per page.
   */
  void refreshDisplay(const size_t pageItems) override;
  /**
   * @brief Sets up signal-slot connections for various UI components.
   */
  void setupConnections() override;  
private:
  /**
   * @brief Refreshes the display for visible categories on the current page.
   * @param visibleCategories A vector of category names to display.
   * @param items The number of items to display.
   */
  void refreshCategoriesDisplay(std::vector<std::string> visibleCategories
      , const size_t items);
  
private slots:
  /**
   * @brief Handles the event when the "Add Category" button is clicked.
   */
  void addCategory_button_clicked();
  
protected slots:
  /**
   * @brief Handles the event when the "Next Page" button is clicked.
   */
  virtual void on_nextPage_button_clicked() override;
  
  /**
   * @brief Handles the event when the "Previous Page" button is clicked.
   */
  virtual void on_previousPage_button_clicked() override;
  
  /**
   * @brief Handles the event when the "Delete" button is clicked for a category.
   */
  virtual void on_delete_button_clicked() override;
  
  /**
   * @brief Handles the event when the "Edit" button is clicked for a category.
   */
  virtual void on_edit_button_clicked() override;
  
  /**
   * @brief Handles the event when the "Categories" button is clicked.
   */
  virtual void on_categories_button_clicked() override;
  
  /**
   * @brief Handles the event when the "Supplies" button is clicked.
   */
  virtual void on_supplies_button_clicked() override;
  
  /**
   * @brief Handles the event when the "Products" button is clicked.
   */
  virtual void on_products_button_clicked() override;
};

#endif // CATEGORIES_H
