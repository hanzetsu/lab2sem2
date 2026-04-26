/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *getButton;
    QPushButton *appendButton;
    QPushButton *prependButton;
    QPushButton *insertAtButton;
    QPushButton *clearButton;
    QListWidget *listWidget;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *valueLineEdit;
    QSpinBox *indexSpinBox;
    QLabel *statusLabel;
    QComboBox *typeComboBox;
    QComboBox *dataTypeComboBox;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(443, 462);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(20, 270, 160, 146));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        getButton = new QPushButton(verticalLayoutWidget);
        getButton->setObjectName("getButton");

        verticalLayout->addWidget(getButton);

        appendButton = new QPushButton(verticalLayoutWidget);
        appendButton->setObjectName("appendButton");

        verticalLayout->addWidget(appendButton);

        prependButton = new QPushButton(verticalLayoutWidget);
        prependButton->setObjectName("prependButton");

        verticalLayout->addWidget(prependButton);

        insertAtButton = new QPushButton(verticalLayoutWidget);
        insertAtButton->setObjectName("insertAtButton");

        verticalLayout->addWidget(insertAtButton);

        clearButton = new QPushButton(verticalLayoutWidget);
        clearButton->setObjectName("clearButton");

        verticalLayout->addWidget(clearButton);

        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(30, 20, 51, 241));
        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(200, 310, 160, 112));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        valueLineEdit = new QLineEdit(verticalLayoutWidget_2);
        valueLineEdit->setObjectName("valueLineEdit");

        verticalLayout_2->addWidget(valueLineEdit);

        indexSpinBox = new QSpinBox(verticalLayoutWidget_2);
        indexSpinBox->setObjectName("indexSpinBox");

        verticalLayout_2->addWidget(indexSpinBox);

        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setGeometry(QRect(30, 0, 158, 16));
        typeComboBox = new QComboBox(centralwidget);
        typeComboBox->setObjectName("typeComboBox");
        typeComboBox->setGeometry(QRect(280, 10, 158, 22));
        dataTypeComboBox = new QComboBox(centralwidget);
        dataTypeComboBox->setObjectName("dataTypeComboBox");
        dataTypeComboBox->setGeometry(QRect(210, 10, 62, 22));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 443, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        getButton->setText(QCoreApplication::translate("MainWindow", "Get", nullptr));
        appendButton->setText(QCoreApplication::translate("MainWindow", "Append", nullptr));
        prependButton->setText(QCoreApplication::translate("MainWindow", "Prepend", nullptr));
        insertAtButton->setText(QCoreApplication::translate("MainWindow", "Insert At", nullptr));
        clearButton->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
