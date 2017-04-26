#ifndef __DEMO11_H_
#define __DEMO11_H_

#include <sceneInitializer.h>

class Demo11 : public SceneInitializer
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
	Demo11(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif