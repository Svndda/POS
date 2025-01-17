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
  Ui::CategoryFormDialog *ui;
  std::vector<std::string> existingCategories;
  QString newCategory;

public:
  explicit CategoryFormDialog(QWidget *parent = nullptr
      , std::vector<std::string> existingCategories
      = std::vector<std::string>());
  ~CategoryFormDialog();
  
public:
  std::string getNewCategory();

};

#endif // CATEGORYFORMDIALOG_H
