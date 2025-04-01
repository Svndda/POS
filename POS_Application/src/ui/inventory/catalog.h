// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#ifndef CATALOG_H
#define CATALOG_H

#include <QWidget>
#include <QPushButton>

#include "model.h"

/**
 * @class Catalog
 * @brief Abstract base class for catalog-related UI components.
 *
 * The Catalog class provides a standardized structure for managing different inventory-related catalogs,
 * such as categories, products, and supplies. It defines common attributes, signals, and slots, while enforcing
 * the implementation of key methods in derived classes.
 */
class Catalog : public QWidget {
  Q_OBJECT
  
public:
  /**
   * @brief Constructs a Catalog object.
   * @param parent Pointer to the parent widget (default is nullptr).
   * @param appModel Reference to the POS_Model instance used for managing application data.
   * @param items Number of items to display per page (default is 9).
   */
  explicit Catalog(QWidget* parent = nullptr
      , Model& appModel = Model::getInstance()
      , const size_t items = 9)
      : QWidget(parent)
      , model(appModel)
      , currentPageIndex(0)
      , itemsPerPage(items) {}
  
  /**
   * @brief Virtual destructor to allow proper cleanup in derived classes.
   */
  virtual ~Catalog() {}
  
protected:
  Model& model;            ///< Reference to the application model.
  size_t currentPageIndex = 0; ///< Index of the currently displayed page.
  size_t itemsPerPage = 9;     ///< Number of items displayed per page.
  
protected:
  /**
   * @brief Pure virtual method to set up signal-slot connections.
   *
   * Derived classes must implement this method to connect UI elements with their corresponding slots.
   */
  virtual void setupConnections() = 0;
  
  /**
   * @brief Pure virtual method to refresh the display of items.
   * @param pageItems Number of items to display per page.
   *
   * Derived classes must implement this method to update the UI based on the current page.
   */
  virtual void refreshDisplay(const size_t pageItems) = 0;
  
protected slots:
  /**
   * @brief Slot for handling the "Next Page" button click event.
   *
   * Must be implemented by derived classes.
   */
  virtual void nextPage_button_clicked() = 0;
  
  /**
   * @brief Slot for handling the "Previous Page" button click event.
   *
   * Must be implemented by derived classes.
   */
  virtual void previousPage_button_clicked() = 0;
  
  /**
   * @brief Slot for handling the "Delete" button click event.
   *
   * Must be implemented by derived classes.
   */
  virtual void delete_button_clicked() = 0;
  
  /**
   * @brief Slot for handling the "Edit" button click event.
   *
   * Must be implemented by derived classes.
   */
  virtual void edit_button_clicked() = 0;
};

#endif // CATALOG_H
