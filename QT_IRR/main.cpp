#include "QT_IRR.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QT_IRR w;
    w.show();

	w.getIrrlichtWidget()->init();

	// Make sure the Irrlicht Device exists before trying to use it
	if (w.getIrrlichtWidget()->getIrrlichtDevice())
	{
		ISceneManager *smgr = w.getIrrlichtWidget()->getIrrlichtDevice()->getSceneManager();
		IVideoDriver *driver = w.getIrrlichtWidget()->getIrrlichtDevice()->getVideoDriver();

		// Just display a simple mesh
		IAnimatedMesh* mesh = smgr->getMesh("./media/sydney.md2");
		if (!mesh)
		{
			w.getIrrlichtWidget()->getIrrlichtDevice()->drop();
			return 1;
		}

		IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
		if (node)
		{
			node->setMaterialFlag(EMF_LIGHTING, false);
			node->setMD2Animation(scene::EMAT_STAND);
			node->setMaterialTexture(0, driver->getTexture("./media/sydney.bmp"));
		}
	}

    return a.exec();
}
