/********************************************************************************
** Form generated from reading UI file 'QT_IRR.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_IRR_H
#define UI_QT_IRR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "QIrrlichtWidget.h"

QT_BEGIN_NAMESPACE

class Ui_QT_IRRClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QIrrlichtWidget *irrWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QT_IRRClass)
    {
        if (QT_IRRClass->objectName().isEmpty())
            QT_IRRClass->setObjectName(QStringLiteral("QT_IRRClass"));
        QT_IRRClass->resize(1047, 858);
        centralWidget = new QWidget(QT_IRRClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        irrWidget = new QIrrlichtWidget(centralWidget);
        irrWidget->setObjectName(QStringLiteral("irrWidget"));

        horizontalLayout->addWidget(irrWidget);

        QT_IRRClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QT_IRRClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1047, 26));
        QT_IRRClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QT_IRRClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QT_IRRClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QT_IRRClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QT_IRRClass->setStatusBar(statusBar);

        retranslateUi(QT_IRRClass);

        QMetaObject::connectSlotsByName(QT_IRRClass);
    } // setupUi

    void retranslateUi(QMainWindow *QT_IRRClass)
    {
        QT_IRRClass->setWindowTitle(QApplication::translate("QT_IRRClass", "QT_IRR", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QT_IRRClass: public Ui_QT_IRRClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_IRR_H
