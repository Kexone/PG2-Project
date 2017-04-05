#ifndef __DEMO8_H_
#define __DEMO8_H_

#include <sceneInitializer.h>

class Demo8 : public SceneInitializer
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
	Demo8(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif