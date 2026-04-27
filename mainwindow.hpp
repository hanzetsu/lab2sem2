#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QString>

#include "Sequence.hpp"
#include "MutableArraySequence.hpp"
#include "MutableListSequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ImmutableListSequence.hpp"
#include "BitSequence.hpp"
#include "Bit.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_typeComboBox_currentIndexChanged(int index);      // тип последовательности (массив/список/биты)
    void on_dataTypeComboBox_currentIndexChanged(int index); // тип данных (int/double/QString)
    void on_appendButton_clicked();
    void on_prependButton_clicked();
    void on_insertAtButton_clicked();
    void on_getButton_clicked();
    void on_clearButton_clicked();
    void updateDisplay();

private:
    Ui::MainWindow *ui;

    // Последовательности для разных типов (активна только одна)
    Sequence<int>* currentIntSequence;
    Sequence<double>* currentDoubleSequence;
    Sequence<QString>* currentStringSequence;
    BitSequence* currentBitSequence;

    int currentSeqType;     // 0=MutableArray,1=MutableList,2=ImmutableArray,3=ImmutableList,4=Bit
    int currentDataType;    // 0=int,1=double,2=QString (актуально только если не Bit)
    bool isBitMode;         // true, если выбрана BitSequence

    void deleteCurrentSequences();
    void createNewSequence();   // создаёт последовательность согласно currentSeqType и currentDataType
};

