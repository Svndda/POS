#ifndef CATEGORIESPAGE_H
#define CATEGORIESPAGE_H

#include <posmodel.h>
#include <QWidget>

namespace Ui {
class CategoriesPage;
}

class CategoriesPage : public QWidget {
  Q_OBJECT
  
private:
  Ui::CategoriesPage *ui;
  POS_Model appModel;
  size_t categoryPageIndex = 0;
  
public:
  explicit CategoriesPage(QWidget *parent = nullptr
      , POS_Model appModel = POS_Model::getInstance());
  ~CategoriesPage();
    
private:
  void refreshCategoriesDisplay(std::vector<std::string> visibleCategories);
  
signals:
  void products_button_signal();
  
private slots:
  void addCategory_button_clicked();
  void on_deleteProduct_button_clicked();
  void on_editProduct_button_clicked();
  void on_nextProductPage_button_clicked();
  void on_previousProductPage_button_clicked();
  void on_products_button_clicked();
};

#endif // CATEGORIESPAGE_H
