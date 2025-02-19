#include "settings.h"
#include "ui_settings.h"

#include "personalization.h"
#include "assistance.h"
#include "logout.h"

Settings::Settings(QWidget *parent, POS_Model& appModel)
    : QWidget(parent)
    , ui(new Ui::Settings)
    , model(appModel) {
  this->ui->setupUi(this);
  this->setupConnections();
  this->prepareDisplay();
}

Settings::~Settings() {
  delete ui;
}

void Settings::setupConnections() {
  this->connect(this->ui->personalization_button, &QPushButton::clicked
      , this, &Settings::on_personalization_button_clicked);
  this->connect(this->ui->assistance_button, &QPushButton::clicked
      , this, &Settings::on_assistance_button_clicked);
  this->connect(this->ui->logout_button, &QPushButton::clicked
      , this, &Settings::on_logout_button_clicked);
}

void Settings::prepareDisplay() {
  this->settingsStack = new QStackedWidget();
  this->settingsStack->setStyleSheet(
      "QStackedWidget {"
      " background-color: white;}");
  
  Personalization* personalizationPage = new Personalization(this, this->model);
  Assistance* assistancePage = new Assistance(this, this->model);
  Logout* logoutPage = new Logout(this, this->model);
  
  this->settingsStack->addWidget(personalizationPage);
  this->settingsStack->addWidget(assistancePage);
  this->settingsStack->addWidget(logoutPage);
  
  this->ui->mainLayout->addWidget(this->settingsStack, 0, 1);
  
  this->switchSettings(1);  
}

void Settings::on_personalization_button_clicked() {
  this->switchSettings(0);
}

void Settings::on_assistance_button_clicked() {
  this->switchSettings(1);
}

void Settings::on_logout_button_clicked() {
  this->switchSettings(2);  
}

void Settings::switchSettings(const size_t index) {
  // Set the current settings page
  this->settingsStack->setCurrentIndex(index);
  
  // Create lists for buttons and corresponding widgets.
  QVector<QPushButton*> buttons = {
      this->ui->personalization_button,
      this->ui->assistance_button,
      this->ui->logout_button
  };
  
  QVector<QWidget*> widgets = {
      this->ui->personalizationIndicator_widget,
      this->ui->assistanceIndicator_widget,
      this->ui->logoutIndicatorwidget
  };
  
  // Iterate over the buttons and widgets to update their states
  for (int i = 0; i < buttons.size(); ++i) {
    const bool isSelected = (i == index);
    buttons[i]->setChecked(isSelected);
    // Use a conditional to set the style sheet: green for selected, transparent otherwise.
    widgets[i]->setStyleSheet(
        QString("QWidget { background-color: %1; }")
            .arg(isSelected ? "rgb(0, 153, 73)" : "transparent")
        );
  }
}
