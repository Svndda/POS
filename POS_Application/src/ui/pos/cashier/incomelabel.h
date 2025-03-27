#ifndef INCOMELABEL_H
#define INCOMELABEL_H

#include <QWidget>

namespace Ui {
class IncomeLabel;
}

class IncomeLabel : public QWidget {
  Q_OBJECT

public:
  explicit IncomeLabel(QWidget *parent = nullptr
                       , const size_t receiptID = 0
                       , const QString time = QString()
                       , const QString paymentMethod = QString()
                       , const double amount = 0);
  ~IncomeLabel();

private:
  Ui::IncomeLabel *ui;
};

#endif // INCOMELABEL_H
