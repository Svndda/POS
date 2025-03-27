#ifndef CATEGORIESCATALOG_H
#define CATEGORIESCATALOG_H

#include <QWidget>

#include "catalog.h"

namespace Ui {
class CategoriesCatalog;
}

/**
 * @class CategoriesCatalog
 * @brief Class for managing the categories catalog in the application.
 *
 * This class inherits from Catalog and implements methods to display, add, edit, and delete categories.
 * It provides a paginated view of the categories available.
 */
class CategoriesCatalog : public Catalog {
  Q_OBJECT

public:
  /**
   * @brief Constructs a CategoriesCatalog object.
   * @param parent Pointer to the parent widget (default is nullptr).
   * @param appModel Reference to the POS_Model instance (default is POS_Model::getInstance()).
   */
  explicit CategoriesCatalog(QWidget* parent = nullptr
      , POS_Model& appModel = POS_Model::getInstance());

  /**
   * @brief Destructor for the CategoriesCatalog class.
   */
  ~CategoriesCatalog();

private:
  Ui::CategoriesCatalog* ui; ///< Pointer to the UI elements for CategoriesCatalog.
  
protected:
  /**
   * @brief Refreshes the category display with the current page of categories.
   * @param pageItems The number of categories to display per page.
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
   * @param items The number of categories to display.
   */
  void refreshCategoriesDisplay(std::vector<std::string> visibleCategories
      , const size_t items);
  
private slots:
  /**
   * @brief Handles the "Add Category" button click event.
   */
  void addCategory_button_clicked();
  
protected slots:
  /**
   * @brief Handles the "Next Page" button click event.
   */
  virtual void nextPage_button_clicked() override;
  
  /**
   * @brief Handles the "Previous Page" button click event.
   */
  virtual void previousPage_button_clicked() override;
  
  /**
   * @brief Handles the "Delete" button click event for a category.
   */
  virtual void delete_button_clicked() override;
  
  /**
   * @brief Handles the "Edit" button click event for a category.
   */
  virtual void edit_button_clicked() override;
  
};

#endif // CATEGORIESCATALOG_H
