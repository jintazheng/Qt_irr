#include "QT_IRR.h"

QT_IRR::QT_IRR(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->irrWidget = ui.irrWidget;
	// Setup for the Irrlicht Widget
	// This is new code that was not in the file generated by QtCreate
	//irrWidget.setParent(ui.centralWidget);
	//irrWidget.setGeometry(0, 0, 600, 800);
}