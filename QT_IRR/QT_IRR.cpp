#include "QT_IRR.h"

QT_IRR::QT_IRR(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->irrWidget = ui.irrWidget;

	//button event
	connect(ui.maya_cam, SIGNAL(clicked()), this, SLOT(handleMayaButton()));
	connect(ui.fps_cam, SIGNAL(clicked()), this, SLOT(handleFpsButton()));
}
// button event function
void QT_IRR::handleMayaButton()
{
	
	
}
void QT_IRR::handleFpsButton()
{
	
}
// key pressed Event
void QT_IRR::keyPressEvent(QKeyEvent *event)
{
	switch(event->key())
	{
	case Qt::Key_E:
		this->close();
		break;
	case Qt::Key_R:
        this->irrWidget->setCamera(Maya);
		break; 
	case Qt::Key_T:
		this->irrWidget->setCamera(FPS);
		break;
	}

}