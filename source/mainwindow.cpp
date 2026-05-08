#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), currentIntSequence(nullptr), currentDoubleSequence(nullptr), currentStringSequence(nullptr), currentBitSequence(nullptr), currentSeqType(0), currentDataType(0), isBitMode(false)
{
    ui->setupUi(this);

    ui->typeComboBox->addItem("MutableArraySequence");
    ui->typeComboBox->addItem("MutableListSequence");
    ui->typeComboBox->addItem("ImmutableArraySequence");
    ui->typeComboBox->addItem("ImmutableListSequence");
    ui->typeComboBox->addItem("BitSequence");

    ui->dataTypeComboBox->addItem("int");
    ui->dataTypeComboBox->addItem("double");
    ui->dataTypeComboBox->addItem("QString");

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
    delete currentStringSequence;
    delete currentBitSequence;
    currentIntSequence = nullptr;
    currentDoubleSequence = nullptr;
    currentStringSequence = nullptr;
    currentBitSequence = nullptr;
}

void MainWindow::createNewSequence()
{
    deleteCurrentSequences();
    if (isBitMode)
    {
        currentBitSequence = new BitSequence();
        return;
    }

    switch (currentSeqType)
    {
    case 0: // MutableArray
        if (currentDataType == 0)
            currentIntSequence = new MutableArraySequence<int>();
        else if (currentDataType == 1)
            currentDoubleSequence = new MutableArraySequence<double>();
        else
            currentStringSequence = new MutableArraySequence<QString>();
        break;
    case 1: // MutableList
        if (currentDataType == 0)
            currentIntSequence = new MutableListSequence<int>();
        else if (currentDataType == 1)
            currentDoubleSequence = new MutableListSequence<double>();
        else
            currentStringSequence = new MutableListSequence<QString>();
        break;
    case 2: // ImmutableArray
        if (currentDataType == 0)
            currentIntSequence = new ImmutableArraySequence<int>();
        else if (currentDataType == 1)
            currentDoubleSequence = new ImmutableArraySequence<double>();
        else
            currentStringSequence = new ImmutableArraySequence<QString>();
        break;
    case 3: // ImmutableList
        if (currentDataType == 0)
            currentIntSequence = new ImmutableListSequence<int>();
        else if (currentDataType == 1)
            currentDoubleSequence = new ImmutableListSequence<double>();
        else
            currentStringSequence = new ImmutableListSequence<QString>();
        break;
    }
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
    if (isBitMode)
        return;
    currentDataType = index;
    createNewSequence();
    updateDisplay();
}

void MainWindow::updateDisplay()
{
    ui->listWidget->clear();

    if (isBitMode && currentBitSequence)
    {
        int len = currentBitSequence->GetLength();
        ui->statusLabel->setText(QString("Длина (биты): %1").arg(len));
        for (int i = 0; i < len; ++i)
        {
            Bit b = currentBitSequence->Get(i);
            ui->listWidget->addItem(b == Bit::one ? "1" : "0");
        }
        return;
    }

    if (currentIntSequence && currentDataType == 0)
    {
        int len = currentIntSequence->GetLength();
        ui->statusLabel->setText(QString("Длина: %1").arg(len));
        for (int i = 0; i < len; ++i)
            ui->listWidget->addItem(QString::number(currentIntSequence->Get(i)));
    }
    else if (currentDoubleSequence && currentDataType == 1)
    {
        int len = currentDoubleSequence->GetLength();
        ui->statusLabel->setText(QString("Длина: %1").arg(len));
        for (int i = 0; i < len; ++i)
            ui->listWidget->addItem(QString::number(currentDoubleSequence->Get(i)));
    }
    else if (currentStringSequence && currentDataType == 2)
    {
        int len = currentStringSequence->GetLength();
        ui->statusLabel->setText(QString("Длина: %1").arg(len));
        for (int i = 0; i < len; ++i)
            ui->listWidget->addItem(currentStringSequence->Get(i));
    }
    else
    {
        ui->statusLabel->setText("Нет последовательности");
    }
}

void MainWindow::on_appendButton_clicked()
{
    QString text = ui->valueLineEdit->text();
    if (text.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Введите значение!");
        return;
    }

    try
    {
        if (isBitMode && currentBitSequence)
        {
            if (text != "0" && text != "1")
            {
                QMessageBox::warning(this, "Ошибка", "Для битов введите 0 или 1");
                return;
            }
            Bit b = (text == "1") ? Bit::one : Bit::zero;
            currentBitSequence->Append(b);
        }
        else
        {
            if (currentDataType == 0 && currentIntSequence)
            {
                bool ok;
                int val = text.toInt(&ok);
                if (!ok)
                    throw std::invalid_argument("Не целое число");
                MutableArraySequence<int> *mArr = dynamic_cast<MutableArraySequence<int> *>(currentIntSequence);
                MutableListSequence<int> *mList = dynamic_cast<MutableListSequence<int> *>(currentIntSequence);
                if (mArr)
                    mArr->Append(val);
                else if (mList)
                    mList->Append(val);
                else
                {
                    Sequence<int> *newSeq = currentIntSequence->Append(val);
                    delete currentIntSequence;
                    currentIntSequence = newSeq;
                }
            }
            else if (currentDataType == 1 && currentDoubleSequence)
            {
                bool ok;
                double val = text.toDouble(&ok);
                if (!ok)
                    throw std::invalid_argument("Не число с плавающей точкой");
                MutableArraySequence<double> *mArr = dynamic_cast<MutableArraySequence<double> *>(currentDoubleSequence);
                MutableListSequence<double> *mList = dynamic_cast<MutableListSequence<double> *>(currentDoubleSequence);
                if (mArr)
                    mArr->Append(val);
                else if (mList)
                    mList->Append(val);
                else
                {
                    Sequence<double> *newSeq = currentDoubleSequence->Append(val);
                    delete currentDoubleSequence;
                    currentDoubleSequence = newSeq;
                }
            }
            else if (currentDataType == 2 && currentStringSequence)
            {
                QString val = text;
                MutableArraySequence<QString> *mArr = dynamic_cast<MutableArraySequence<QString> *>(currentStringSequence);
                MutableListSequence<QString> *mList = dynamic_cast<MutableListSequence<QString> *>(currentStringSequence);
                if (mArr)
                    mArr->Append(val);
                else if (mList)
                    mList->Append(val);
                else
                {
                    Sequence<QString> *newSeq = currentStringSequence->Append(val);
                    delete currentStringSequence;
                    currentStringSequence = newSeq;
                }
            }
        }
        updateDisplay();
        ui->valueLineEdit->clear();
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Ошибка при Append", e.what());
    }
}
void MainWindow::on_prependButton_clicked()
{
    QString text = ui->valueLineEdit->text();
    if (text.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Введите значение!");
        return;
    }

    try
    {
        if (isBitMode && currentBitSequence)
        {
            if (text != "0" && text != "1")
            {
                QMessageBox::warning(this, "Ошибка", "Для битов введите 0 или 1");
                return;
            }
            Bit b = (text == "1") ? Bit::one : Bit::zero;
            Sequence<Bit> *newSeq = currentBitSequence->Prepend(b);
            delete currentBitSequence;
            currentBitSequence = static_cast<BitSequence *>(newSeq);
        }
        else
        {
            if (currentDataType == 0 && currentIntSequence)
            {
                bool ok;
                int val = text.toInt(&ok);
                if (!ok)
                    throw std::invalid_argument("Не целое число");
                MutableArraySequence<int> *mArr = dynamic_cast<MutableArraySequence<int> *>(currentIntSequence);
                MutableListSequence<int> *mList = dynamic_cast<MutableListSequence<int> *>(currentIntSequence);
                if (mArr)
                    mArr->Prepend(val);
                else if (mList)
                    mList->Prepend(val);
                else
                {
                    Sequence<int> *newSeq = currentIntSequence->Prepend(val);
                    delete currentIntSequence;
                    currentIntSequence = newSeq;
                }
            }
            else if (currentDataType == 1 && currentDoubleSequence)
            {
                bool ok;
                double val = text.toDouble(&ok);
                if (!ok)
                    throw std::invalid_argument("Не число с плавающей точкой");
                MutableArraySequence<double> *mArr = dynamic_cast<MutableArraySequence<double> *>(currentDoubleSequence);
                MutableListSequence<double> *mList = dynamic_cast<MutableListSequence<double> *>(currentDoubleSequence);
                if (mArr)
                    mArr->Prepend(val);
                else if (mList)
                    mList->Prepend(val);
                else
                {
                    Sequence<double> *newSeq = currentDoubleSequence->Prepend(val);
                    delete currentDoubleSequence;
                    currentDoubleSequence = newSeq;
                }
            }
            else if (currentDataType == 2 && currentStringSequence)
            {
                QString val = text;
                MutableArraySequence<QString> *mArr = dynamic_cast<MutableArraySequence<QString> *>(currentStringSequence);
                MutableListSequence<QString> *mList = dynamic_cast<MutableListSequence<QString> *>(currentStringSequence);
                if (mArr)
                    mArr->Prepend(val);
                else if (mList)
                    mList->Prepend(val);
                else
                {
                    Sequence<QString> *newSeq = currentStringSequence->Prepend(val);
                    delete currentStringSequence;
                    currentStringSequence = newSeq;
                }
            }
        }
        updateDisplay();
        ui->valueLineEdit->clear();
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Ошибка при Prepend", e.what());
    }
}
void MainWindow::on_insertAtButton_clicked()
{
    int index = ui->indexSpinBox->value();
    QString text = ui->valueLineEdit->text();
    if (text.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Введите значение!");
        return;
    }

    try
    {
        if (isBitMode && currentBitSequence)
        {
            if (text != "0" && text != "1")
            {
                QMessageBox::warning(this, "Ошибка", "Для битов введите 0 или 1");
                return;
            }
            Bit b = (text == "1") ? Bit::one : Bit::zero;
            Sequence<Bit> *newSeq = currentBitSequence->InsertAt(b, index);
            delete currentBitSequence;
            currentBitSequence = static_cast<BitSequence *>(newSeq);
        }
        else
        {
            if (currentDataType == 0 && currentIntSequence)
            {
                bool ok;
                int val = text.toInt(&ok);
                if (!ok)
                    throw std::invalid_argument("Не целое число");
                MutableArraySequence<int> *mArr = dynamic_cast<MutableArraySequence<int> *>(currentIntSequence);
                MutableListSequence<int> *mList = dynamic_cast<MutableListSequence<int> *>(currentIntSequence);
                if (mArr)
                    mArr->InsertAt(val, index);
                else if (mList)
                    mList->InsertAt(val, index);
                else
                {
                    Sequence<int> *newSeq = currentIntSequence->InsertAt(val, index);
                    delete currentIntSequence;
                    currentIntSequence = newSeq;
                }
            }
            else if (currentDataType == 1 && currentDoubleSequence)
            {
                bool ok;
                double val = text.toDouble(&ok);
                if (!ok)
                    throw std::invalid_argument("Не число с плавающей точкой");
                MutableArraySequence<double> *mArr = dynamic_cast<MutableArraySequence<double> *>(currentDoubleSequence);
                MutableListSequence<double> *mList = dynamic_cast<MutableListSequence<double> *>(currentDoubleSequence);
                if (mArr)
                    mArr->InsertAt(val, index);
                else if (mList)
                    mList->InsertAt(val, index);
                else
                {
                    Sequence<double> *newSeq = currentDoubleSequence->InsertAt(val, index);
                    delete currentDoubleSequence;
                    currentDoubleSequence = newSeq;
                }
            }
            else if (currentDataType == 2 && currentStringSequence)
            {
                QString val = text;
                MutableArraySequence<QString> *mArr = dynamic_cast<MutableArraySequence<QString> *>(currentStringSequence);
                MutableListSequence<QString> *mList = dynamic_cast<MutableListSequence<QString> *>(currentStringSequence);
                if (mArr)
                    mArr->InsertAt(val, index);
                else if (mList)
                    mList->InsertAt(val, index);
                else
                {
                    Sequence<QString> *newSeq = currentStringSequence->InsertAt(val, index);
                    delete currentStringSequence;
                    currentStringSequence = newSeq;
                }
            }
        }
        updateDisplay();
        ui->valueLineEdit->clear();
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Ошибка при InsertAt", e.what());
    }
}
void MainWindow::on_getButton_clicked()
{
    int index = ui->indexSpinBox->value();
    try
    {
        if (isBitMode && currentBitSequence)
        {
            Bit b = currentBitSequence->Get(index);
            QMessageBox::information(this, "Элемент", QString("Бит на позиции %1: %2").arg(index).arg(b == Bit::one ? "1" : "0"));
        }
        else if (currentDataType == 0 && currentIntSequence)
        {
            int val = currentIntSequence->Get(index);
            QMessageBox::information(this, "Элемент", QString("Значение на позиции %1: %2").arg(index).arg(val));
        }
        else if (currentDataType == 1 && currentDoubleSequence)
        {
            double val = currentDoubleSequence->Get(index);
            QMessageBox::information(this, "Элемент", QString("Значение на позиции %1: %2").arg(index).arg(val));
        }
        else if (currentDataType == 2 && currentStringSequence)
        {
            QString val = currentStringSequence->Get(index);
            QMessageBox::information(this, "Элемент", QString("Значение на позиции %1: %2").arg(index).arg(val));
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Нет активной последовательности");
        }
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Ошибка при получении элемента", e.what());
    }
}
void MainWindow::on_clearButton_clicked()
{
    if (isBitMode && currentBitSequence)
    {
        delete currentBitSequence;
        currentBitSequence = new BitSequence();
    }
    else
    {
        createNewSequence();
    }
    updateDisplay();
}