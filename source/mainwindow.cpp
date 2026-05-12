#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), currentIntSequence(nullptr),
      currentDoubleSequence(nullptr), currentBitSequence(nullptr),
      currentSeqType(0), currentDataType(0), isBitMode(false)
{
    ui->setupUi(this);
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->typeComboBox->addItem("MutableArraySequence");
    ui->typeComboBox->addItem("MutableListSequence");
    ui->typeComboBox->addItem("ImmutableArraySequence");
    ui->typeComboBox->addItem("ImmutableListSequence");
    ui->typeComboBox->addItem("BitSequence");

    ui->dataTypeComboBox->addItem("int");
    ui->dataTypeComboBox->addItem("double");

    currentSeqType = 0;
    currentDataType = 0;
    createNewSequence();

    ui->dataTypeComboBox->setEnabled(!isBitMode);
    on_typeComboBox_currentIndexChanged(0);
}

MainWindow::~MainWindow()
{
    deleteCurrentSequences();
    delete ui;
}

void MainWindow::deleteCurrentSequences()
{
    delete currentIntSequence;
    delete currentDoubleSequence;
    delete currentBitSequence;
    currentIntSequence = nullptr;
    currentDoubleSequence = nullptr;
    currentBitSequence = nullptr;
}

void MainWindow::createNewSequence()
{
    deleteCurrentSequences();
    if (isBitMode) {
        currentBitSequence = new BitSequence();
        updateInsertAtButtonState();
        return;
    }

    switch (currentSeqType) {
    case 0: // MutableArray
        if (currentDataType == 0) currentIntSequence = new MutableArraySequence<int>();
        else currentDoubleSequence = new MutableArraySequence<double>();
        break;
    case 1: // MutableList
        if (currentDataType == 0) currentIntSequence = new MutableListSequence<int>();
        else currentDoubleSequence = new MutableListSequence<double>();
        break;
    case 2: // ImmutableArray
        if (currentDataType == 0) currentIntSequence = new ImmutableArraySequence<int>();
        else currentDoubleSequence = new ImmutableArraySequence<double>();
        break;
    case 3: // ImmutableList
        if (currentDataType == 0) currentIntSequence = new ImmutableListSequence<int>();
        else currentDoubleSequence = new ImmutableListSequence<double>();
        break;
    }
    updateInsertAtButtonState();
}

void MainWindow::updateInsertAtButtonState()
{
    bool empty = false;
    if (isBitMode)
        empty = (currentBitSequence == nullptr || currentBitSequence->GetLength() == 0);
    else if (currentDataType == 0)
        empty = (currentIntSequence == nullptr || currentIntSequence->GetLength() == 0);
    else
        empty = (currentDoubleSequence == nullptr || currentDoubleSequence->GetLength() == 0);
    ui->insertAtButton->setEnabled(!empty);
}

void MainWindow::on_typeComboBox_currentIndexChanged(int index)
{
    currentSeqType = index;
    isBitMode = (index == 4);
    ui->dataTypeComboBox->setEnabled(!isBitMode);
    createNewSequence();
    updateDisplay();
}

void MainWindow::on_dataTypeComboBox_currentIndexChanged(int index)
{
    if (isBitMode) return;
    currentDataType = index;
    createNewSequence();
    updateDisplay();
}

void MainWindow::updateDisplay()
{
    ui->listWidget->clear();
    updateInsertAtButtonState();

    if (isBitMode && currentBitSequence) {
        std::size_t len = currentBitSequence->GetLength();
        ui->statusLabel->setText(QString("Длина (биты): %1").arg(len));
        for (std::size_t i = 0; i < len; ++i) {
            Bit b = currentBitSequence->Get(i);
            ui->listWidget->addItem(b == Bit::one ? "1" : "0");
        }
        return;
    }

    if (currentIntSequence && currentDataType == 0) {
        std::size_t len = currentIntSequence->GetLength();
        ui->statusLabel->setText(QString("Длина: %1").arg(len));
        for (std::size_t i = 0; i < len; ++i)
            ui->listWidget->addItem(QString::number(currentIntSequence->Get(i)));
    }
    else if (currentDoubleSequence && currentDataType == 1) {
        std::size_t len = currentDoubleSequence->GetLength();
        ui->statusLabel->setText(QString("Длина: %1").arg(len));
        for (std::size_t i = 0; i < len; ++i)
            ui->listWidget->addItem(QString::number(currentDoubleSequence->Get(i)));
    }
    else {
        ui->statusLabel->setText("Нет последовательности");
    }
}

void MainWindow::on_appendButton_clicked()
{
    QString text = ui->valueLineEdit->text();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите значение!");
        return;
    }

    try {
        if (isBitMode && currentBitSequence) {
            if (text != "0" && text != "1") {
                QMessageBox::warning(this, "Ошибка", "Для битов введите 0 или 1");
                return;
            }
            Bit b = (text == "1") ? Bit::one : Bit::zero;
            currentBitSequence->Append(b);
        } else if (currentDataType == 0 && currentIntSequence) {
            bool ok; int val = text.toInt(&ok);
            if (!ok) throw InvalidArgument("Не целое число");
            appendToSequence(currentIntSequence, val);
        } else if (currentDataType == 1 && currentDoubleSequence) {
            bool ok; double val = text.toDouble(&ok);
            if (!ok) throw InvalidArgument("Не число с плавающей точкой");
            appendToSequence(currentDoubleSequence, val);
        }
        updateDisplay();
        ui->valueLineEdit->clear();
    } catch (const IException& e) {
        QMessageBox::critical(this, "Ошибка при Append", e.what());
    }
}

void MainWindow::on_prependButton_clicked()
{
    QString text = ui->valueLineEdit->text();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите значение!");
        return;
    }

    try {
        if (isBitMode && currentBitSequence) {
            if (text != "0" && text != "1") {
                QMessageBox::warning(this, "Ошибка", "Для битов введите 0 или 1");
                return;
            }
            Bit b = (text == "1") ? Bit::one : Bit::zero;
            Sequence<Bit> *newSeq = currentBitSequence->Prepend(b);
            delete currentBitSequence;
            currentBitSequence = static_cast<BitSequence *>(newSeq);
        } else if (currentDataType == 0 && currentIntSequence) {
            bool ok; int val = text.toInt(&ok);
            if (!ok) throw InvalidArgument("Не целое число");
            prependToSequence(currentIntSequence, val);
        } else if (currentDataType == 1 && currentDoubleSequence) {
            bool ok; double val = text.toDouble(&ok);
            if (!ok) throw InvalidArgument("Не число с плавающей точкой");
            prependToSequence(currentDoubleSequence, val);
        }
        updateDisplay();
        ui->valueLineEdit->clear();
    } catch (const IException& e) {
        QMessageBox::critical(this, "Ошибка при Prepend", e.what());
    }
}

void MainWindow::on_insertAtButton_clicked()
{
    int indexInt = ui->indexSpinBox->value();
    if (indexInt < 0) {
        QMessageBox::warning(this, "Ошибка", "Индекс не может быть отрицательным");
        return;
    }
    std::size_t index = static_cast<std::size_t>(indexInt);

    QString text = ui->valueLineEdit->text();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите значение!");
        return;
    }

    try {
        if (isBitMode && currentBitSequence) {
            if (text != "0" && text != "1") {
                QMessageBox::warning(this, "Ошибка", "Для битов введите 0 или 1");
                return;
            }
            Bit b = (text == "1") ? Bit::one : Bit::zero;
            Sequence<Bit> *newSeq = currentBitSequence->InsertAt(b, index);
            delete currentBitSequence;
            currentBitSequence = static_cast<BitSequence *>(newSeq);
        } else if (currentDataType == 0 && currentIntSequence) {
            bool ok; int val = text.toInt(&ok);
            if (!ok) throw InvalidArgument("Не целое число");
            insertToSequence(currentIntSequence, val, index);
        } else if (currentDataType == 1 && currentDoubleSequence) {
            bool ok; double val = text.toDouble(&ok);
            if (!ok) throw InvalidArgument("Не число с плавающей точкой");
            insertToSequence(currentDoubleSequence, val, index);
        }
        updateDisplay();
        ui->valueLineEdit->clear();
    } catch (const IException& e) {
        QMessageBox::critical(this, "Ошибка при InsertAt", e.what());
    }
}

void MainWindow::on_clearButton_clicked()
{
    if (isBitMode && currentBitSequence) {
        delete currentBitSequence;
        currentBitSequence = new BitSequence();
    } else {
        createNewSequence();
    }
    updateDisplay();
}

void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = ui->listWidget->itemAt(pos);
    if (!item) return;

    QMenu menu;
    QAction *removeAction = menu.addAction("Удалить элемент");
    QAction *editAction = menu.addAction("Редактировать элемент");
    QAction *selected = menu.exec(ui->listWidget->mapToGlobal(pos));
    if (selected == removeAction) removeSelected();
    else if (selected == editAction) editSelected();
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (item) editSelected();
}

void MainWindow::removeSelected()
{
    int row = ui->listWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите элемент для удаления");
        return;
    }
    std::size_t index = static_cast<std::size_t>(row);

    try {
        if (isBitMode && currentBitSequence) {
            std::size_t len = currentBitSequence->GetLength();
            if (index >= len) throw IndexOutOfRange(index, len, "BitSequence::remove");
            BitSequence *newSeq = new BitSequence();
            for (std::size_t i = 0; i < len; ++i) {
                if (i == index) continue;
                newSeq->Append(currentBitSequence->Get(i));
            }
            delete currentBitSequence;
            currentBitSequence = newSeq;
        } else if (currentDataType == 0 && currentIntSequence) {
            removeFromSequence(currentIntSequence, index);
        } else if (currentDataType == 1 && currentDoubleSequence) {
            removeFromSequence(currentDoubleSequence, index);
        }
        updateDisplay();
    } catch (const IException& e) {
        QMessageBox::critical(this, "Ошибка при удалении", e.what());
    }
}

void MainWindow::editSelected()
{
    int row = ui->listWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите элемент для редактирования");
        return;
    }
    std::size_t index = static_cast<std::size_t>(row);

    QString oldValueText;
    if (isBitMode && currentBitSequence) {
        oldValueText = (currentBitSequence->Get(index) == Bit::one) ? "1" : "0";
    } else if (currentDataType == 0 && currentIntSequence) {
        oldValueText = QString::number(currentIntSequence->Get(index));
    } else if (currentDataType == 1 && currentDoubleSequence) {
        oldValueText = QString::number(currentDoubleSequence->Get(index));
    } else {
        QMessageBox::warning(this, "Ошибка", "Нет активной последовательности");
        return;
    }

    bool ok;
    QString newText = QInputDialog::getText(this, "Редактирование", "Новое значение:",
                                            QLineEdit::Normal, oldValueText, &ok);
        if (newText.isEmpty()) QMessageBox::warning(this, "Ошибка","Пусто");
    if (!ok || newText.isEmpty()) return;

    try {
        if (isBitMode && currentBitSequence) {
            if (newText != "0" && newText != "1") {
                QMessageBox::warning(this, "Ошибка", "Для битов введите 0 или 1");
                return;
            }
            Bit b = (newText == "1") ? Bit::one : Bit::zero;
            std::size_t len = currentBitSequence->GetLength();
            if (index >= len) throw IndexOutOfRange(index, len, "BitSequence::edit");
            BitSequence *newSeq = new BitSequence();
            for (std::size_t i = 0; i < len; ++i) {
                Bit val = (i == index) ? b : currentBitSequence->Get(i);
                newSeq->Append(val);
            }
            delete currentBitSequence;
            currentBitSequence = newSeq;
        } else if (currentDataType == 0 && currentIntSequence) {
            bool convOk; int val = newText.toInt(&convOk);
            if (!convOk) throw InvalidArgument("Не целое число");
            editInSequence(currentIntSequence, index, val);
        } else if (currentDataType == 1 && currentDoubleSequence) {
            bool convOk; double val = newText.toDouble(&convOk);
            if (!convOk) throw InvalidArgument("Не число с плавающей точкой");
            editInSequence(currentDoubleSequence, index, val);
        }
        updateDisplay();
    } catch (const IException& e) {
        QMessageBox::critical(this, "Ошибка при редактировании", e.what());
    }
}