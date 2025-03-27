#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include "posmodel.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget {
  Q_OBJECT
  
private:
private:
  Ui::Settings* ui;               ///< Pointer to the UI elements for the Settings class.
  POS_Model& model;                ///< Reference to the POS model instance.
  QStackedWidget* settingsStack;    ///< Pointer to the stack of inventory's catalog pages.
  
public:
  /**
   * @brief Constructor for the Settings class.
   * 
   * @param parent The parent QWidget (default is nullptr).
   * @param model Reference to the singleton POS_Model instance.
   */
  explicit Settings(QWidget *parent = nullptr
      , POS_Model& model = POS_Model::getInstance());
  
  /**
   * @brief Destructor for the Settings class.
   */
  ~Settings();
  
private:
  void setupConnections();
  
  void prepareDisplay();
  
  void switchSettings(const size_t index);
  
  void on_personalization_button_clicked();
  
  void on_assistance_button_clicked();
  
  void on_logout_button_clicked();
  
  void logouUser();
  
signals:
  void logoutCurrentUser();
};

#endif // SETTINGS_H
