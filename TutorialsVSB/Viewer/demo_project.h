#ifndef __DEMO_PROJECT_H_
#define __DEMO_PROJECT_H_

#include <sceneInitializer.h>
#include "Particle.h"
#include <vector>
#include "ParticleSystem.h"

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
	vector<Particle> particles;
	ParticleSystem ps;
	GLuint nr_particles = 500;

public:
	DemoProject(SceneData *sdPtr) : SceneInitializer(sdPtr) {}
	GLuint particlesVAO;
	void render();
};

#endif