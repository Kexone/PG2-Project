#ifndef __DEMO6_1_H_
#define __DEMO6_1_H_

#include <sceneInitializer.h>

class Demo6_1 : public SceneInitializer
{
private:
	void initShaders();
	void initModels();
	void initMaterials();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();
	void initTextures();
	int static const width = 200, height = 200;

public:
	Demo6_1(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif