#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>

namespace Ui {
class InventoryPage;
}

class Inventory : public QWidget
{
  Q_OBJECT

public:
  explicit Inventory(QWidget *parent = nullptr);
  ~Inventory();

private:
  Ui::InventoryPage *ui;
};

#endif // INVENTORY_H
