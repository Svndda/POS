#ifndef POS_H
#define POS_H

#include <QWidget>

namespace Ui {
class Pos;
}

class Pos : public QWidget
{
  Q_OBJECT

public:
  explicit Pos(QWidget *parent = nullptr);
  ~Pos();

private:
  Ui::Pos *ui;
};

#endif // POS_H
