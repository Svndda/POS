// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef SUPPLIESCATALOG_H
#define SUPPLIESCATALOG_H

#include "catalog.h"

namespace Ui {
class SuppliesCatalog;
}

/**
 * @class SuppliesCatalog
 * @brief Class for managing the supplies catalog in the application.
 *
 * This class inherits from Catalog and implements methods to display, add, edit, and delete supply items.
 * It provides a paginated view of the supplies available.
 */
class SuppliesCatalog : public Catalog {
  Q_OBJECT
  
private:
  Ui::SuppliesCatalog* ui; ///< Pointer to the UI elements for SuppliesCatalog.
  
public:
  /**
   * @brief Constructs a SuppliesCatalog object.
   * @param parent Pointer to the parent widget (default is nullptr).
   * @param appModel Reference to the POS_Model instance (default is POS_Model::getInstance()).
   */
  explicit SuppliesCatalog(QWidget *parent = nullptr
      , Model& appModel = Model::getInstance());
  
  /**
   * @brief Destructor for the SuppliesCatalog  class.
   */
  ~SuppliesCatalog();
  
protected:
  /**
   * @brief Refreshes the display with the supplies of the current page.
   * @param pageItems Number of supplies to display per page.
   */
  void refreshDisplay(const size_t pageItems) override;
  
  /**
   * @brief Sets up signal-slot connections for the UI components.
   */
  void setupConnections() override;
  
private:
  /**
   * @brief Updates the UI to display the supplies on the current page.
   * @param visibleSupplies Vector of Supply objects representing the supplies to be displayed.
   * @param items Number of items to display.
   */
  void refreshSuppliesDisplay(const std::vector<Supply>& visibleSupplies
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
  virtual void nextPage_button_clicked() override;
  
  /**
   * @brief Slot triggered when the "Previous Page" button is clicked.
   */
  virtual void previousPage_button_clicked() override;
  
  /**
   * @brief Slot triggered when the "Delete Supply" button is clicked.
   */
  virtual void delete_button_clicked() override;
  
  /**
   * @brief Slot triggered when the "Edit Supply" button is clicked.
   */
  virtual void edit_button_clicked() override;
};

#endif // SUPPLIESCATALOG_H
