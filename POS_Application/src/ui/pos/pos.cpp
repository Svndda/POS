#include "pos.h"
#include "ui_pos.h"

Pos::Pos(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Pos)
{
  ui->setupUi(this);
}

Pos::~Pos()
{
  delete ui;
}
