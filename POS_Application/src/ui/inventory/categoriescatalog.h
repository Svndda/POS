#ifndef CATEGORIESCATALOG_H
#define CATEGORIESCATALOG_H

#include <QWidget>

#include "catalog.h"

namespace Ui {
class CategoriesCatalog;
}

class CategoriesCatalog : public Catalog {
  Q_OBJECT

public:
  explicit CategoriesCatalog(QWidget* parent = nullptr
      , POS_Model& appModel = POS_Model::getInstance());
  ~CategoriesCatalog();

private:
  Ui::CategoriesCatalog* ui;
  
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
  
};

#endif // CATEGORIESCATALOG_H
