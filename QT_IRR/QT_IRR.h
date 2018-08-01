#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QT_IRR.h"
#include "QIrrlichtWidget.h"
class QT_IRR : public QMainWindow
{
    Q_OBJECT

public:
    QT_IRR(QWidget *parent = Q_NULLPTR);

	QIrrlichtWidget* getIrrlichtWidget(){ return irrWidget; }

private:
    Ui::QT_IRRClass ui;
	QIrrlichtWidget* irrWidget;
};
