#ifndef __DEMO_PROJECT_H_
#define __DEMO_PROJECT_H_

#include <sceneInitializer.h>

class DemoProject : public SceneInitializer
{
private:
	void initShaders();
	void initModels();
	void initParticles();
	void initTextures();
	void initMaterials();
	void initVAOs();
	void initInfoEntities();
	void initSceneEntities();

public:
	DemoProject(SceneData *sdPtr) : SceneInitializer(sdPtr) {}
	GLuint particlesVAO;
	float position;
	bool changeDir;
	void render();
};

#endif