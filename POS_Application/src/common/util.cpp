// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include "util.h"

#include <QString>
#include <QWidget>
#include <QDebug>

void Util::updatePrefixedLabel(QWidget* view,  const QString& labelPrefix
    , const size_t labelIt, const QString& value) {
  
  // Generates the label name based on the suffix and label index.
  QString labelName = QString("%1_label_%2").arg(labelPrefix).arg(labelIt);
  QLabel* label = view->findChild<QLabel*>(labelName);
  
  // Checks if catched a label with that name.
  if (label != nullptr) {
    // Updates the label text with the given text value.
    label->setText(value.trimmed());
    label->setAlignment(Qt::AlignCenter);
  } else {
    qDebug() << "No se pudo encontrar el puntero del label con prefijo: "
        << labelPrefix;
  }
}

QLabel* Util::cloneLabel(QLabel* original) {
  QLabel* newLabel = new QLabel(original->parentWidget());
  
  // Copy the important attributes.
  newLabel->setText(original->text());
  newLabel->setAlignment(original->alignment());
  newLabel->setStyleSheet(original->styleSheet());
  newLabel->setFont(original->font());
  newLabel->setPixmap(original->pixmap(Qt::ReturnByValue)); // Si usa pixmap
  
  // Other settings if needed.
  newLabel->setSizePolicy(original->sizePolicy());
  newLabel->setGeometry(original->geometry());
  
  return newLabel;
}
