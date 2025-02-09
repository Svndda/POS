// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include "posmodel.h"

namespace Ui {
class Inventory;
}

/**
 * @class Products
 * @brief Handles the user interface and logic for managing products in the POS system.
 * 
 * This class extends the Inventory interface to display, add, edit, and delete 
 * products using a paginated UI. It uses a model (`POS_Model`) to interact with 
 * the underlying data.
 */
class Inventory : public QWidget {
  Q_OBJECT
  
private:
  Ui::Inventory* ui;               ///< Pointer to the UI elements for the Products class.
  POS_Model& model;                ///< Reference to the POS model instance.
  QStackedWidget* catalogStack;    ///< Pointer to the stack of inventory's catalog pages.

public:
  /**
   * @brief Constructor for the Products class.
   * 
   * @param parent The parent QWidget (default is nullptr).
   * @param model Reference to the singleton POS_Model instance.
   */
  explicit Inventory(QWidget *parent = nullptr
      , POS_Model& model = POS_Model::getInstance());
  
  /**
   * @brief Destructor for the Products class.
   */
  ~Inventory();
  
protected:
  /**
   * @brief Refreshes the display with the products of the current page.
   * 
   * @param pageItems Number of items to display per page.
   */
  void refreshDisplay(const size_t pageItems);
  
  /**
   * @brief Sets up connections between UI elements and their respective slots.
   */
  void setupConnections();
  
  /**
   * @brief Switches the current catalog view.
   *
   * This function updates the displayed catalog page in the QStackedWidget based on the provided index.
   * It also updates the state of the catalog buttons and their corresponding widgets by setting the
   * appropriate "checked" status and applying a style sheet.
   *
   * @param index The index of the catalog to display (0 for Products, 1 for Categories, 2 for Supplies).
   */
  void switchCatalog(const size_t index);
  
private slots:
  /**
   * @brief Slot for handling the "ProductsCatalog" button click event.
   */
  void on_productsCatalog_button_clicked();
  
  /**
   * @brief Slot for handling the "CategoriesCatalog" button click event.
   */
  void on_categoriesCatalog_button_clicked();
  
  /**
   * @brief Slot for handling the "SuppliesCatalog" button click event.
   */
  void on_suppliesCatalog_button_clicked();
  
};

#endif // INVENTORY_H
