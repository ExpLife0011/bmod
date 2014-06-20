#ifndef BMOD_DISASSEMBLER_DIALOG_H
#define BMOD_DISASSEMBLER_DIALOG_H

#include <QDialog>

#include "../CpuType.h"

class QTextEdit;
class QSplitter;
class QLineEdit;
class QComboBox;
class QPushButton;

class DisassemblerDialog : public QDialog {
  Q_OBJECT

public:
  DisassemblerDialog(QWidget *parent = nullptr,
                     CpuType cpuType = CpuType::X86,
                     const QString &data = QString(),
                     quint64 offset = 0);

private slots:
  void onConvert();

private:
  void createLayout();
  void setAsmVisible(bool visible = true);

  CpuType cpuType;
  quint64 offset;

  QTextEdit *machineText, *asmText;
  QSplitter *splitter;
  QLineEdit *offsetEdit;
  QComboBox *cpuTypeBox;
  QPushButton *convertBtn;
};

#endif // BMOD_DISASSEMBLER_DIALOG_H
