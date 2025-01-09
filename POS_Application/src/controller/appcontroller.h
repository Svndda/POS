#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QMainWindow>
#include <QStackedWidget>
#include "posmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class AppController : public QMainWindow {
  Q_OBJECT

public:
  AppController(QWidget *parent = nullptr);
  ~AppController();

private:
  Ui::MainWindow* ui = nullptr;
  QStackedWidget* pageStack = nullptr;
  POS_Model& model = POS_Model::getInstance();

private:
  void on_btnSubmit_clicked();
};
#endif // APPCONTROLLER_H
