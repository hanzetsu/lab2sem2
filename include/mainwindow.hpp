#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QListWidgetItem>

#include "Sequence.hpp"
#include "MutableArraySequence.hpp"
#include "MutableListSequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ImmutableListSequence.hpp"
#include "BitSequence.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_typeComboBox_currentIndexChanged(int index);
    void on_dataTypeComboBox_currentIndexChanged(int index);
    void on_appendButton_clicked();
    void on_prependButton_clicked();
    void on_insertAtButton_clicked();
    void on_clearButton_clicked();
    void on_listWidget_customContextMenuRequested(const QPoint &pos);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void updateDisplay();

private:
    Ui::MainWindow *ui;

    Sequence<int>* currentIntSequence;
    Sequence<double>* currentDoubleSequence;
    BitSequence* currentBitSequence;

    int currentSeqType;
    int currentDataType;
    bool isBitMode;

    void deleteCurrentSequences();
    void createNewSequence();
    void updateInsertAtButtonState();
    void removeSelected();
    void editSelected();

    template<typename T>
    void appendToSequence(Sequence<T>*& seq, T value);
    template<typename T>
    void prependToSequence(Sequence<T>*& seq, T value);
    template<typename T>
    void insertToSequence(Sequence<T>*& seq, T value, std::size_t index);
    template<typename T>
    void removeFromSequence(Sequence<T>*& seq, std::size_t index);
    template<typename T>
    void editInSequence(Sequence<T>*& seq, std::size_t index, T newValue);
};

#include "mainwindow_impl.hpp"