// Copyright [2025] Aaron Carmona Sanchez <aaroncz032@gmail.com>
#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include "model.h"

namespace Ui {
class Inventory;
}

/**
 * @class Inventory
 * @brief Handles the user interface and logic for managing the inventoy in the POS system.
 * 
 * This class extends the Inventory interface to display the different registered catalogs in the system
 * using a paginated UI. It uses a pos model (`Model`) to interact with 
 * the underlying data.
 */
class Inventory : public QWidget {
  Q_OBJECT
  
private:
  Ui::Inventory* ui;               ///< Pointer to the UI elements for the Products class.
  Model& model;                ///< Reference to the Model instance.
  QStackedWidget* catalogStack;    ///< Pointer to the stack of inventory's catalog pages.

public:
  /**
   * @brief Constructor for the Inventory class.
   * 
   * @param parent The parent QWidget (default is nullptr).
   * @param model Reference to the singleton Model instance.
   */
  explicit Inventory(QWidget *parent = nullptr
      , Model& model = Model::getInstance());
  
  /**
   * @brief Destructor for the Inventory class.
   */
  ~Inventory();
  
protected:
  /**
   * @brief Refreshes the display with the elements of the current page.
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
