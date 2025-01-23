// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef SUPPLIES_H
#define SUPPLIES_H

#include "inventory.h"

namespace Ui {
class Supplies;
}

/**
 * @class Supplies
 * @brief Manages the supplies UI and logic in the POS system.
 * 
 * This class extends the Inventory interface to display, add, edit, and delete 
 * supply items using a paginated UI.
 */
class Supplies : public Inventory {
  Q_OBJECT
  
private:
  Ui::Supplies* ui; ///< Pointer to the UI elements for the Supplies class.
  
public:
  /**
   * @brief Constructor for the Supplies class.
   * 
   * @param parent The parent QWidget (default is nullptr).
   * @param appModel Reference to the singleton POS_Model instance.
   */
  explicit Supplies(QWidget *parent = nullptr, POS_Model& appModel = POS_Model::getInstance());
  
  /**
   * @brief Destructor for the Supplies class.
   */
  ~Supplies();
  
protected:
  /**
   * @brief Refreshes the display with the supplies of the current page.
   * 
   * @param pageItems Number of items to display per page.
   */
  void refreshDisplay(const size_t pageItems) override;
  
  /**
   * @brief Sets up connections between UI elements and their respective slots.
   */
  void setupConnections() override;
private:
  /**
   * @brief Refreshes the UI to display supplies on the current page.
   * 
   * @param visibleSupplies Vector of supplies to be displayed.
   * @param items Number of items to display.
   */
  void refreshSuppliesDisplay(const std::vector<SupplyItem>& visibleSupplies
      , const size_t items);
  
private slots:
  /**
   * @brief Slot triggered when the "Add Supply" button is clicked.
   */
  void addSupply_button_clicked();
  
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
   * @brief Slot triggered when the "Delete Supply" button is clicked.
   */
  virtual void on_delete_button_clicked() override;
  
  /**
   * @brief Slot triggered when the "Edit Supply" button is clicked.
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

#endif // SUPPLIES_H
