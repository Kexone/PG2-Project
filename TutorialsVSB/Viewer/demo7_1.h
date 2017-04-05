#ifndef __DEMO7_H_
#define __DEMO7_H_

#include <sceneInitializer.h>

class Demo7_1 : public SceneInitializer
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
	Demo7_1(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif