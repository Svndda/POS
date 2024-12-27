#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>

namespace Ui {
class DashboardPage;
}

class Dashboard : public QWidget
{
  Q_OBJECT

public:
  explicit Dashboard(QWidget *parent = nullptr);
  ~Dashboard();

private:
  Ui::DashboardPage *ui;
};

#endif // DASHBOARD_H
