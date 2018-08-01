// QIrrlichtWidget.h

#ifndef QIRRLICHTWIDGET_H
#define QIRRLICHTWIDGET_H

#include <QWidget>
#include <QResizeEvent>
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
enum Cam_type{Maya, FPS};
// Our Irrlicht rendering widget
// To have everything compile, we need to run MOC on this file
class QIrrlichtWidget : public QWidget
{
	// Macro for the meta-object compiler MOC
	Q_OBJECT

public:
	explicit QIrrlichtWidget(QWidget *parent = 0);
	~QIrrlichtWidget();

	// Returns a pointer to the Irrlicht Device
	IrrlichtDevice* getIrrlichtDevice();
	ISceneManager * getIrrlichtSceneManager();
	IVideoDriver*  getIrrlichtVideoDriver();
	IGUIEnvironment* getIrrlichtGUIEnvironment();
	// Create the Irrlicht device and connect the signals and slots
	void init();

	void setActiveCamera(scene::ICameraSceneNode* newActive);
	void updateScaleInfo(scene::ISceneNode* model);
	void loadModel(const c8* fn);
	void onKillFocus();
	
	void setCamera(Cam_type mode)
	{
		if (Maya == mode)
		{
			device->getCursorControl()->setVisible(true);
			this->setActiveCamera(Camera[0]);
		}
		if (FPS == mode)
		{
			device->getCursorControl()->setVisible(false);
			this->setActiveCamera(Camera[1]);
		}
	}

signals:
	// Signal that its time to update the frame
	void updateIrrlichtQuery(IrrlichtDevice* device);

	public slots:
	// Function called in response to updateIrrlichtQuery. Renders the scene in the widget
	void updateIrrlicht(IrrlichtDevice* device);



protected:
	virtual void paintEvent(QPaintEvent* event);
	virtual void timerEvent(QTimerEvent* event);
	virtual void resizeEvent(QResizeEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

	//Qt event to irrlicht
	void sendKeyEventToIrrlicht(QKeyEvent* event, bool pressedDown);
	void sendMouseEventToIrrlicht(QMouseEvent* event, bool pressedDown);
	

	IrrlichtDevice *device;
	ISceneManager* smgr;
	IVideoDriver* driver;
	IGUIEnvironment* env;
	ICameraSceneNode* Camera[2];

	stringc StartUpModelFile;
	stringw MessageText;
	stringw Caption;
	ISceneNode* Model = 0;
	ISceneNode* SkyBox = 0;
	bool Octree = false;
	bool UseLight = false;
	bool irrhasFocus;
	
};

#endif // QIRRWIDGET_H
