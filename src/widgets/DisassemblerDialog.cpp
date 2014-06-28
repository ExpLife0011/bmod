#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QSplitter>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include "../Util.h"
#include "../BinaryObject.h"
#include "DisassemblerDialog.h"
#include "../asm/Disassembler.h"

DisassemblerDialog::DisassemblerDialog(QWidget *parent, CpuType cpuType,
                                       const QString &data, quint64 offset)
  : QDialog{parent}, cpuType{cpuType}, offset{offset}
{
  setWindowTitle(tr("Disassembler"));
  createLayout();
  resize(400, 300);
  Util::centerWidget(this);

  if (!data.isEmpty()) {
    machineText->setText(data);
    convertBtn->click();
  }
}

void DisassemblerDialog::onConvert() {
  QString text = machineText->toPlainText();
  if (text.isEmpty()) {
    setAsmVisible(false);
    machineText->setFocus();
    QMessageBox::warning(this, "bmod", tr("Write some machine code!"));
    return;
  }

  quint64 offset = offsetEdit->text().toULongLong(nullptr, 16);

  auto obj = BinaryObjectPtr(new BinaryObject(cpuType));
  Disassembler dis(obj);
  Disassembly result;
  if (dis.disassemble(text, result, offset)) {
    asmText->setText(result.asmLines.join("\n"));
    setAsmVisible();
  }
  else {
    setAsmVisible(false);
    machineText->setFocus();
    QMessageBox::warning(this, "bmod",
                         tr("Could not disassemble machine code!"));
  }
}

void DisassemblerDialog::onCpuTypeIndexChanged(int index) {
  cpuType = (CpuType) cpuTypeBox->itemData(index).toInt();
}

void DisassemblerDialog::createLayout() {
  machineText = new QTextEdit;
  machineText->setTabChangesFocus(true);

  offsetEdit = new QLineEdit;
  offsetEdit->setText(QString::number(offset, 16));
  offsetEdit->setAlignment(Qt::AlignRight);
  offsetEdit->setFixedWidth(140);
  offsetEdit->setValidator(new QRegExpValidator(QRegExp("[A-Fa-f0-9]{1,16}"), this));

  auto *offsetLayout = new QHBoxLayout;
  offsetLayout->setContentsMargins(0, 0, 0, 0);
  offsetLayout->addWidget(new QLabel(tr("Hex offset:")));
  offsetLayout->addWidget(offsetEdit);
  offsetLayout->addStretch();

  auto *machineLayout = new QVBoxLayout;
  machineLayout->setContentsMargins(0, 0, 0, 0);
  machineLayout->addWidget(new QLabel(tr("Machine code:")));
  machineLayout->addWidget(machineText);
  machineLayout->addLayout(offsetLayout);

  auto *machineWidget = new QWidget;
  machineWidget->setLayout(machineLayout);

  asmText = new QTextEdit;
  asmText->setReadOnly(true);
  asmText->setTabChangesFocus(true);

  auto *asmLayout = new QVBoxLayout;
  asmLayout->setContentsMargins(0, 0, 0, 0);
  asmLayout->addWidget(new QLabel(tr("Disassembly:")));
  asmLayout->addWidget(asmText);

  auto *asmWidget = new QWidget;
  asmWidget->setLayout(asmLayout);

  splitter = new QSplitter(Qt::Vertical);
  splitter->addWidget(machineWidget);
  splitter->addWidget(asmWidget);

  splitter->setCollapsible(0, false);
  splitter->setCollapsible(1, true);
  setAsmVisible(false);

  cpuTypeBox = new QComboBox;
  cpuTypeBox->addItem(tr("X86"), (int) CpuType::X86);
  cpuTypeBox->addItem(tr("X86_64"), (int) CpuType::X86_64);

  int idx = cpuTypeBox->findData((int) cpuType);
  if (idx != -1) {
    cpuTypeBox->setCurrentIndex(idx);
  }

  connect(cpuTypeBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(onCpuTypeIndexChanged(int)));

  convertBtn = new QPushButton(tr("Disassemble"));
  connect(convertBtn, &QPushButton::clicked,
          this, &DisassemblerDialog::onConvert);

  auto *bottomLayout = new QHBoxLayout;
  bottomLayout->addWidget(new QLabel(tr("CPU:")));
  bottomLayout->addWidget(cpuTypeBox);
  bottomLayout->addStretch();
  bottomLayout->addWidget(convertBtn);
  
  auto *layout = new QVBoxLayout;
  layout->setContentsMargins(5, 5, 5, 5);
  layout->addWidget(splitter);
  layout->addLayout(bottomLayout);
  
  setLayout(layout);
}

void DisassemblerDialog::setAsmVisible(bool visible) {
  splitter->setSizes(QList<int>{1, visible ? 1 : 0});
  if (!visible) asmText->clear();
}
