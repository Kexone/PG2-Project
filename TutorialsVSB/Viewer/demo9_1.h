#ifndef __DEMO9_H_
#define __DEMO9_H_

#include <sceneInitializer.h>

class Demo9_1 : public SceneInitializer
{
private:
	void initShaders();
	void initModels();
	void initTextures();
	void initMaterials();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();

public:
	Demo9_1(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif