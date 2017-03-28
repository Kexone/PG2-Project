#ifndef __DEMO3_H_
#define __DEMO3_H_

#include <sceneInitializer.h>

class Demo3 : public SceneInitializer
{
private:
	void initShaders();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();

public:
	Demo3(SceneData *sdPtr) : SceneInitializer(sdPtr) {}

	void render();
};

#endif