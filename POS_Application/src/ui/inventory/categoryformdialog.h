#ifndef CATEGORYFORMDIALOG_H
#define CATEGORYFORMDIALOG_H

#include <QDialog>
#include <QString>
#include <vector>
#include <string>

namespace Ui {
class CategoryFormDialog;
}

class CategoryFormDialog : public QDialog {
  Q_OBJECT
  
private:
  Ui::CategoryFormDialog* ui;
  std::vector<std::string> existingCategories;
  std::string newCategory;

public:
  explicit CategoryFormDialog(QWidget* parent
      , std::vector<std::string> categories
      , std::string category);
  ~CategoryFormDialog();
  
public:
  std::string getNewCategory();
  
private slots:
  void on_acceptCategory_button_clicked();
  void on_cancelCategory_button_clicked();
  
};

#endif // CATEGORYFORMDIALOG_H
