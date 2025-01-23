// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>
#include <QPushButton>

#include "posmodel.h"

/**
 * @class Inventory
 * @brief Abstract base class that serves as an interface for inventory-related subpages in the application.
 * 
 * The `Inventory` class provides a standardized structure for managing different inventory-related subpages,
 * such as categories, products, and supplies. It defines common attributes, signals, and slots, while enforcing
 * implementation of key methods in derived classes.
 */
class Inventory : public QWidget {
  Q_OBJECT
  
public:
  /**
   * @brief Constructs an Inventory object.
   * @param parent The parent widget, defaulting to nullptr.
   * @param appModel A reference to the POS_Model instance used for managing application data.
   * @param items The number of items to display per page, defaulting to 9.
   */
  explicit Inventory(QWidget* parent = nullptr
      , POS_Model& appModel = POS_Model::getInstance()
      , const size_t items = 9)
      : QWidget(parent)
      , model(appModel)
      , currentPageIndex(0)
      , itemsPerPage(items) {}
  
  /**
   * @brief Virtual destructor to allow proper cleanup in derived classes.
   */
  virtual ~Inventory() {}
  
protected:
  /**
   * @brief Reference to the application model, which provides data for the inventory.
   */
  POS_Model& model;
  /**
   * @brief Index of the currently displayed page in the inventory.
   */
  size_t currentPageIndex = 0;
  /**
   * @brief Number of items displayed per page in the inventory.
   */
  size_t itemsPerPage = 9;
  
protected:
  /**
   * @brief Pure virtual method that must be implemented by derived classes to establish signal-slot connections.
   */
  virtual void setupConnections() = 0;
  /**
   * @brief Pure virtual method that must be implemented by derived classes to refresh the displayed items.
   * @param pageItems The number of items to display per page.
   */
  virtual void refreshDisplay(const size_t pageItems) = 0;
  
public:
signals:
  /**
   * @brief Signal emitted when the "Products" button is clicked.
   */
  void products_button_signal();
  
  /**
   * @brief Signal emitted when the "Categories" button is clicked.
   */
  void categories_button_signal();
  
  /**
   * @brief Signal emitted when the "Supplies" button is clicked.
   */
  void supplies_button_signal();
  
protected slots:
  /**
   * @brief Slot for handling the "Next Page" button click event.
   * This method can be overridden by derived classes.
   */
  virtual void on_nextPage_button_clicked() {}
  
  /**
   * @brief Slot for handling the "Previous Page" button click event.
   * This method can be overridden by derived classes.
   */
  virtual void on_previousPage_button_clicked() {}
  
  /**
   * @brief Slot for handling the "Delete" button click event.
   * This method can be overridden by derived classes.
   */
  virtual void on_delete_button_clicked() {}
  
  /**
   * @brief Slot for handling the "Edit" button click event.
   * This method can be overridden by derived classes.
   */
  virtual void on_edit_button_clicked() {}
  
  /**
   * @brief Slot for handling the "Categories" button click event.
   * This method can be overridden by derived classes.
   */
  virtual void on_categories_button_clicked() {}
  
  /**
   * @brief Slot for handling the "Supplies" button click event.
   * This method can be overridden by derived classes.
   */
  virtual void on_supplies_button_clicked() {}
  
  /**
   * @brief Slot for handling the "Products" button click event.
   * This method can be overridden by derived classes.
   */
  virtual void on_products_button_clicked() {}
};

#endif // INVENTORY_H
