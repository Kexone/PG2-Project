#ifndef __DEMO6Vase_H_
#define __DEMO6Vase_H_

#include <sceneInitializer.h>

class Demo6Vase : public SceneInitializer
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
	Demo6Vase(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif