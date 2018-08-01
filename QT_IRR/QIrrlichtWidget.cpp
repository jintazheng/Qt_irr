// QIrrlichtWidget.cpp
#include <QtCore/QDebug>
#include "QIrrlichtWidget.h"


QIrrlichtWidget::QIrrlichtWidget(QWidget *parent) :
QWidget(parent)
{
	this->setMouseTracking(true);
	this->grabKeyboard();
	// Indicates that the widget wants to draw directly onto the screen. (From documentation : http://doc.qt.nokia.com/latest/qt.html)
	// Essential to have this or there will be nothing displayed
	this->setAttribute(Qt::WA_PaintOnScreen);
	// Indicates that the widget paints all its pixels when it receives a paint event.
	// Thus, it is not required for operations like updating, resizing, scrolling and focus changes to erase the widget before generating paint events.
	// Not sure this is required for the program to run properly, but it is here just incase.
	this->setAttribute(Qt::WA_OpaquePaintEvent);
	// Widget accepts focus by both tabbing and clicking
	this->setFocusPolicy(Qt::StrongFocus);
	// Not sure if this is necessary, but it was in the code I am basing this solution off of
	this->setAutoFillBackground(false);



	this->device = 0;
	this->init();
}

QIrrlichtWidget::~QIrrlichtWidget()
{
	if (device != 0)
	{
		device->closeDevice();
		device->drop();
	}
}

// Create the Irrlicht device and connect the signals and slots
void QIrrlichtWidget::init()
{
	// Make sure we can't create the device twice
	if (device != 0)
		return;

	// Set all the device creation parameters
	SIrrlichtCreationParameters params;
	params.AntiAlias = 0;
	params.Bits = 32;
	params.DeviceType = EIDT_WIN32;
	params.Doublebuffer = true;
	params.DriverType = EDT_OPENGL;
	params.EventReceiver = 0;
	//params.EventReceiver = &receiver;
	params.Fullscreen = false;
	params.HighPrecisionFPU = false;
	params.IgnoreInput = false;
	params.LoggingLevel = ELL_INFORMATION;
	params.Stencilbuffer = true;
	params.Stereobuffer = false;
	params.Vsync = false;
	// Specify which window/widget to render to
	params.WindowId = reinterpret_cast<void*>(winId());
	params.WindowSize.Width = width();
	params.WindowSize.Height = height();
	params.WithAlphaChannel = false;
	params.ZBufferBits = 16;


	
	// Create the Irrlicht Device with the previously specified parameters
	this->device = createDeviceEx(params);
	


	if (device)
	{
		//get driver, scene, env
		this->smgr = device->getSceneManager();
		this->driver = device->getVideoDriver();;
		this->env = device->getGUIEnvironment();
	}

	this->device->setResizable(true);
	this->smgr->getParameters()->setAttribute(scene::COLLADA_CREATE_SCENE_INSTANCES, true);
	this->driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
	this->smgr->addLightSceneNode(0, core::vector3df(200, 200, 200), video::SColorf(1.0f, 1.0f, 1.0f), 2000);
	this->smgr->setAmbientLight(video::SColorf(0.3f, 0.3f, 0.3f));
	// add our media directory as "search path"
	this->device->getFileSystem()->addFileArchive("./media/");

	// set camera
	// add a camera scene node
	// Maya cameras reposition themselves relative to their target, so target the location
	// where the mesh scene node is placed.
	this->Camera[0] = smgr->addCameraSceneNodeMaya(0, -30, 100, 10);
	this->Camera[0]->setPosition(core::vector3df(0, 0, -70));
    this->Camera[0]->setTarget(core::vector3df(0, 30, 0));

	this->Camera[1] = smgr->addCameraSceneNodeFPS(0, 10, 0.2);
	this->Camera[1]->setPosition(core::vector3df(0, 0, -70));
	this->Camera[1]->setTarget(core::vector3df(0, 30, 0));

	this->setActiveCamera(Camera[0]);
	device->getCursorControl()->setVisible(false);
	this->irrhasFocus = this->hasFocus();

	// read configuration from xml file
	IXMLReader* xml = this->device->getFileSystem()->createXMLReader(L"config.xml");

	while (xml && xml->read())
	{
		switch (xml->getNodeType())
		{
		case io::EXN_TEXT:
			// in this xml file, the only text which occurs is the
			// messageText
			MessageText = xml->getNodeData();
			break;
		case io::EXN_ELEMENT:
		{
				if (core::stringw("startUpModel") == xml->getNodeName())
					StartUpModelFile = xml->getAttributeValue(L"file");
				else if (core::stringw("messageText") == xml->getNodeName())
					Caption = xml->getAttributeValue(L"caption");
		}
			break;
		default:
			break;
		}
	}

	if (xml)
		xml->drop(); // don't forget to delete the xml reader

	loadModel(StartUpModelFile.c_str());

	// add skybox
	SkyBox = smgr->addSkyBoxSceneNode(
		driver->getTexture("irrlicht2_up.jpg"),
		driver->getTexture("irrlicht2_dn.jpg"),
		driver->getTexture("irrlicht2_lf.jpg"),
		driver->getTexture("irrlicht2_rt.jpg"),
		driver->getTexture("irrlicht2_ft.jpg"),
		driver->getTexture("irrlicht2_bk.jpg"));

	// Connect the update signal (updateIrrlichtQuery) to the update slot (updateIrrlicht)
	connect(this, SIGNAL(updateIrrlichtQuery(IrrlichtDevice*)), this, SLOT(updateIrrlicht(IrrlichtDevice*)));
	// Start a timer. A timer with setting 0 will update as often as possible.
	startTimer(0);
}

IrrlichtDevice* QIrrlichtWidget::getIrrlichtDevice()
{
	return device;
}
ISceneManager * QIrrlichtWidget::getIrrlichtSceneManager()
{
	return smgr;
}
IVideoDriver*  QIrrlichtWidget::getIrrlichtVideoDriver()
{
	return driver;
}
IGUIEnvironment* QIrrlichtWidget::getIrrlichtGUIEnvironment()
{
	return env;
}

void QIrrlichtWidget::paintEvent(QPaintEvent* event)
{
	if (device != 0)
	{
		emit updateIrrlichtQuery(device);
	}
}
void QIrrlichtWidget::timerEvent(QTimerEvent* event)
{
	// Emit the render signal each time the timer goes off
	if (device != 0)
	{
		emit updateIrrlichtQuery(device);
	}

	event->accept();
}
void QIrrlichtWidget::resizeEvent(QResizeEvent* event)
{
	if (device != 0)
	{
		dimension2d<u32> widgetSize;
		widgetSize.Width = event->size().width();
		widgetSize.Height = event->size().height();
		device->getVideoDriver()->OnResize(widgetSize);

		ICameraSceneNode *cam = device->getSceneManager()->getActiveCamera();
		if (cam != 0)
		{
			cam->setAspectRatio((f32)widgetSize.Height / (f32)widgetSize.Width);
		}
	}

	QWidget::resizeEvent(event);
}
void QIrrlichtWidget::updateIrrlicht(irr::IrrlichtDevice* device)
{
	if (device != 0)
	{
		device->getTimer()->tick();

		// Catch focus changes (workaround until Irrlicht has events for this)
		bool focused = this->hasFocus();
		if (irrhasFocus && !focused)
			onKillFocus();
		irrhasFocus = focused;
	
		if (this->isActiveWindow())
		{
			driver->beginScene(true, true, video::SColor(150, 50, 50, 50));

			smgr->drawAll();
			driver->endScene();
		}
		else
			device->yield();

	}

}

///Event Recever from Qt to irrlicht////////////////////////////////////////////////////////////
struct SIrrlichtKey
{
	irr::EKEY_CODE code;
	wchar_t ch;
};
SIrrlichtKey convertToIrrlichtKey(int key)
{
	SIrrlichtKey irrKey;
	irrKey.code = (irr::EKEY_CODE)(0);
	irrKey.ch = (wchar_t)(0);

	// Letters A..Z and numbers 0..9 are mapped directly
	if ((key >= Qt::Key_A && key <= Qt::Key_Z) || (key >= Qt::Key_0 && key <= Qt::Key_9))
	{
		irrKey.code = (irr::EKEY_CODE)(key);
		irrKey.ch = (wchar_t)(key);
	}
	else

		// Dang, map keys individually
		switch (key)
	{
		case Qt::Key_Up:
			irrKey.code = irr::KEY_UP;
			break;

		case Qt::Key_Down:
			irrKey.code = irr::KEY_DOWN;
			break;

		case Qt::Key_Left:
			irrKey.code = irr::KEY_LEFT;
			break;

		case Qt::Key_Right:
			irrKey.code = irr::KEY_RIGHT;
			break;
	}
	return irrKey;
}

void QIrrlichtWidget::sendKeyEventToIrrlicht(QKeyEvent* event, bool pressedDown)
{
	irr::SEvent irrEvent;

	irrEvent.EventType = irr::EET_KEY_INPUT_EVENT;

	SIrrlichtKey irrKey = convertToIrrlichtKey(event->key());

	if (irrKey.code == 0) return; // Could not find a match for this key

	irrEvent.KeyInput.Key = irrKey.code;
	irrEvent.KeyInput.Control = ((event->modifiers() & Qt::ControlModifier) != 0);
	irrEvent.KeyInput.Shift = ((event->modifiers() & Qt::ShiftModifier) != 0);
	irrEvent.KeyInput.Char = irrKey.ch;
	irrEvent.KeyInput.PressedDown = pressedDown;

	device->postEventFromUser(irrEvent);
}
void QIrrlichtWidget::keyPressEvent(QKeyEvent* event)
{
	if (device != 0)
	{
		sendKeyEventToIrrlicht(event, true);
	}
	event->ignore();
}
void QIrrlichtWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (device != 0)
	{
		sendKeyEventToIrrlicht(event, false);
	}
	event->ignore();
}
void QIrrlichtWidget::sendMouseEventToIrrlicht(QMouseEvent* event, bool pressedDown)
{
	irr::SEvent irrEvent;

	irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

	switch (event->button())
	{
	case Qt::LeftButton:
		irrEvent.MouseInput.Event = pressedDown ? irr::EMIE_LMOUSE_PRESSED_DOWN : irr::EMIE_LMOUSE_LEFT_UP;
		break;                                         

	case Qt::MidButton:
		irrEvent.MouseInput.Event = pressedDown ? irr::EMIE_MMOUSE_PRESSED_DOWN : irr::EMIE_MMOUSE_LEFT_UP;
		break;

	case Qt::RightButton:
		irrEvent.MouseInput.Event = pressedDown ? irr::EMIE_RMOUSE_PRESSED_DOWN : irr::EMIE_RMOUSE_LEFT_UP;
		break;

	default:
		return; // Cannot handle this mouse event
	}

	irrEvent.MouseInput.X = event->x();
	irrEvent.MouseInput.Y = event->y();
	irrEvent.MouseInput.Wheel = 0.0f; // Zero is better than undefined
	device->postEventFromUser(irrEvent);
}
void QIrrlichtWidget::mouseMoveEvent(QMouseEvent* event)
{
	irr::SEvent irrEvent;

	irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

	if (device != 0)
	{
		irrEvent.MouseInput.Event = irr::EMIE_MOUSE_MOVED;

		irrEvent.MouseInput.X = event->x();
		irrEvent.MouseInput.Y = event->y();
		irrEvent.MouseInput.Wheel = 0.0f; // Zero is better than undefined
		device->postEventFromUser(irrEvent);
	}
	event->ignore();
};
void QIrrlichtWidget::mousePressEvent(QMouseEvent* event)
{
	if (device != 0)
	{
		sendMouseEventToIrrlicht(event, true);
	}
	event->ignore();
}
void QIrrlichtWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (device != 0)
	{
		sendMouseEventToIrrlicht(event, false);
	}
	event->ignore();
}
void QIrrlichtWidget::wheelEvent(QWheelEvent* event)
{
	if (device != 0 && event->orientation() == Qt::Vertical)
	{
		irr::SEvent irrEvent;

		irrEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;

		irrEvent.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;
		irrEvent.MouseInput.X = 0; // We don't know these,
		irrEvent.MouseInput.Y = 0; // but better zero them instead of letting them be undefined
		irrEvent.MouseInput.Wheel = event->delta() / 120.0f;

		device->postEventFromUser(irrEvent);
	}
	event->ignore();
}
/*
tool
*/
/*
Toggle between various cameras
*/
void QIrrlichtWidget::setActiveCamera(scene::ICameraSceneNode* newActive)
{
	if (0 == this->device)
		return;

	scene::ICameraSceneNode * active = this->device->getSceneManager()->getActiveCamera();
	active->setInputReceiverEnabled(false);

	newActive->setInputReceiverEnabled(true);
	this->device->getSceneManager()->setActiveCamera(newActive);
}


/*
Update the display of the model scaling
*/
void QIrrlichtWidget::updateScaleInfo(scene::ISceneNode* model)
{

}



/*
Function loadModel() loads a model and displays it using an
addAnimatedMeshSceneNode and the scene manager. Nothing difficult. It also
displays a short message box, if the model could not be loaded.
*/
void QIrrlichtWidget::loadModel(const c8* fn)
{
	// modify the name if it a .pk3 file

	io::path filename(fn);

	io::path extension;
	core::getFileNameExtension(extension, filename);
	extension.make_lower();

	// if a texture is loaded apply it to the current model..
	if (extension == ".jpg" || extension == ".pcx" ||
		extension == ".png" || extension == ".ppm" ||
		extension == ".pgm" || extension == ".pbm" ||
		extension == ".psd" || extension == ".tga" ||
		extension == ".bmp" || extension == ".wal" ||
		extension == ".rgb" || extension == ".rgba")
	{
		video::ITexture * texture =
			this->device->getVideoDriver()->getTexture(filename);
		if (texture && Model)
		{
			// always reload texture
			this->device->getVideoDriver()->removeTexture(texture);
			texture = this->device->getVideoDriver()->getTexture(filename);

			Model->setMaterialTexture(0, texture);
		}
		return;
	}
	// if a archive is loaded add it to the FileArchive..
	else if (extension == ".pk3" || extension == ".zip" || extension == ".pak" || extension == ".npk")
	{
		this->device->getFileSystem()->addFileArchive(filename.c_str());
		return;
	}

	// load a model into the engine

	if (Model)
		Model->remove();

	Model = 0;

	if (extension == ".irr")
	{
		core::array<scene::ISceneNode*> outNodes;
		this->device->getSceneManager()->loadScene(filename);
		this->device->getSceneManager()->getSceneNodesFromType(scene::ESNT_ANIMATED_MESH, outNodes);
		if (outNodes.size())
			Model = outNodes[0];
		return;
	}

	scene::IAnimatedMesh* m = this->device->getSceneManager()->getMesh(filename.c_str());

	if (!m)
	{
		// model could not be loaded

		if (StartUpModelFile != filename)
			this->device->getGUIEnvironment()->addMessageBox(
			Caption.c_str(), L"The model could not be loaded. " \
			L"Maybe it is not a supported file format.");
		return;
	}

	// set default material properties

	if (Octree)
		Model = this->device->getSceneManager()->addOctreeSceneNode(m->getMesh(0));
	else
	{
		scene::IAnimatedMeshSceneNode* animModel = this->device->getSceneManager()->addAnimatedMeshSceneNode(m);
		animModel->setAnimationSpeed(30);
		Model = animModel;
	}
	Model->setMaterialFlag(video::EMF_LIGHTING, UseLight);
	Model->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, UseLight);
	//	Model->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	Model->setDebugDataVisible(scene::EDS_OFF);


	updateScaleInfo(Model);
}



void QIrrlichtWidget::onKillFocus()
{
	// Avoid that the FPS-camera continues moving when the user presses alt-tab while 
	// moving the camera. 
	const core::list<scene::ISceneNodeAnimator*>& animators = Camera[1]->getAnimators();
	core::list<irr::scene::ISceneNodeAnimator*>::ConstIterator iter = animators.begin();
	while (iter != animators.end())
	{
		if ((*iter)->getType() == scene::ESNAT_CAMERA_FPS)
		{
			// we send a key-down event for all keys used by this animator
			scene::ISceneNodeAnimatorCameraFPS * fpsAnimator = static_cast<scene::ISceneNodeAnimatorCameraFPS*>(*iter);
			const core::array<SKeyMap>& keyMap = fpsAnimator->getKeyMap();
			for (irr::u32 i = 0; i< keyMap.size(); ++i)
			{
				irr::SEvent event;
				event.EventType = EET_KEY_INPUT_EVENT;
				event.KeyInput.Key = keyMap[i].KeyCode;
				event.KeyInput.PressedDown = false;
				fpsAnimator->OnEvent(event);
			}
		}
		++iter;
	}
}