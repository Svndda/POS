// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
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

/**
 * @class AppController
 * @brief The main controller class for managing the application workflow.
 *
 * This class acts as the primary controller for the application, managing 
 * page transitions and interactions with the POS model.
 */
class AppController : public QMainWindow {
  Q_OBJECT

public:
  /**
   * @brief Constructs the main application controller.
   * @param parent Pointer to the parent widget.
   */
  AppController(QWidget* parent = nullptr);
  
  /**
   * @brief Destroys the application controller and cleans up resources.
   */
  ~AppController();

private:
  Ui::MainWindow* ui;                  ///< Pointer to the main UI layout.
  QStackedWidget* pageStack;           ///< Stack widget managing application pages.
  POS_Model& model;                    ///< Reference to the POS model instance.
  
private:
  void setupConnections();
  void switchPages(const size_t pageIndex);
  
private slots:
  void on_pos_button_clicked();
  void on_inventory_button_clicked();
  void on_sells_button_clicked();
  void on_users_button_clicked();
  void on_settings_button_clicked();
};
#endif // APPCONTROLLER_H
