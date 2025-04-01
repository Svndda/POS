// Copyright [2025] Aaron Carmona Sanchez <aaron.carmona@ucr.ac.cr>
#include "util.h"

#include <QString>
#include <QWidget>
#include <QDebug>
#include <QDialog>
#include <QPainter>
#include <QPrintdialog>
#include <QFiledialog>
#include <QPrinter>
#include <QPrinterinfo>
#include <QMessageBox>

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

std::unique_ptr<QPrinter> Util::systemPrinter(
    QWidget* parent, const QString& preferredPrinterName) {
  // Create printer object
  auto printer = std::make_unique<QPrinter>();
  
  // Check available printers
  const auto availablePrinters = QPrinterInfo::availablePrinters();
  
  // Try to find preferred printer
  bool preferredPrinterFound = false;
  for (const auto& printerInfo : availablePrinters) {
    if (printerInfo.printerName().compare(
            preferredPrinterName, Qt::CaseInsensitive) == 0) {
      qDebug() << "✅ Printer" << preferredPrinterName << "is available";
      printer->setPrinterName(printerInfo.printerName());
      preferredPrinterFound = true;
      break;
    }
  }
  
  // If preferred printer not found, show print dialog
  if (!preferredPrinterFound) {
    qDebug() << "Preferred printer not found, showing print dialog";
    QPrintDialog printDialog(printer.get(), parent);
    
    if (printDialog.exec() != QDialog::Accepted) {
      qDebug() << "Printing canceled by user";
      return nullptr;
    }
  }
  
  // Validate printer
  if (!printer->isValid()) {
    QMessageBox::critical(parent, "Printer Error"
        ,"No valid printer was selected.");
    return nullptr;
  }
  
  // Handle PDF printer case
  if (printer->printerName().contains("PDF", Qt::CaseInsensitive)) {
    QString filePath = QFileDialog::getSaveFileName(
        parent, "Save Receipt as PDF", "", "PDF Files (*.pdf)");
    
    if (filePath.isEmpty()) {
      qDebug() << "User canceled PDF file selection";
      return nullptr;
    }

    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOutputFileName(filePath);
  } else {
    printer->setOutputFormat(QPrinter::NativeFormat);
  }
  
  // Configure printer settings for receipts
  printer->setResolution(84);  // Standard thermal printer resolution
  printer->setPageSize(
      QPageSize(QSizeF(Util::getPrintableWidth(*printer),
      500), QPageSize::Millimeter)
  );
  printer->setPageMargins(QMarginsF(0, 0, 0, 0), QPageLayout::Millimeter);
  printer->setFullPage(true);
  
  qDebug() << "Printer configured successfully - Name:"
      << printer->printerName()
      << "| Output:" << printer->outputFileName()
      << "| Format:"
      << (printer->outputFormat() == QPrinter::PdfFormat ? "PDF" : "Native");
  
  return printer;
}

qreal Util::getPrintableWidth(QPrinter& printer) {
  // Get the physical page size from the printer
  QPageLayout pageLayout = printer.pageLayout();
  QSizeF pageSize = pageLayout.fullRectPixels(printer.resolution()).size();
  
  // Convert pixels to millimeters (considering the printer resolution)
  qreal pixelsPerMm = printer.resolution() / 25.4; // 25.4mm = 1 inch
  qreal widthMm = pageSize.width() / pixelsPerMm;
  
  // For thermal printers, we typically use a standard width
  const qreal DEFAULT_THERMAL_WIDTH = 80.0; // 80mm is standard for receipt printers
  
  // If we get an unreasonable value, return the default
  if (widthMm <= 0 || widthMm > 1000) {
    qDebug() << "Using default width of" << DEFAULT_THERMAL_WIDTH << "mm";
    return DEFAULT_THERMAL_WIDTH;
  }
  
  qDebug() << "Calculated printable width:" << widthMm << "mm";
  return widthMm;
}

/**
 * @brief Prints a line with word wrapping
 * @param painter Painter to use
 * @param text Text to print
 * @param y Current y position (updated by function)
 * @param bold Whether to print in bold
 * @param metrics Font metrics for calculations
 * @param margin Page margins
 * @param contentWidth Available content width
 * @param lineHeight Height of each line
 */
void Util::printWrappedLine(
    QPainter& painter, const QString& text, int& y,
    const bool bold, const QFontMetrics& metrics, 
    const int margin, const int contentWidth, 
    const int lineHeight) {
  
  painter.setFont(
      bold ? QFont("Courier", 10, QFont::Bold) : QFont("Courier", 10)
  );
  
  QStringList wrappedLines;
  QString currentLine;
  
  for (const QChar& c : text) {
    currentLine += c;
    if (metrics.horizontalAdvance(currentLine) > contentWidth) {
      wrappedLines.append(currentLine);
      currentLine.clear();
    }
  }
  
  if (!currentLine.isEmpty()) {
    wrappedLines.append(currentLine);
  }
  
  for (const QString& line : wrappedLines) {
    painter.drawText(margin, y, line);
    y += lineHeight;
  }
}
