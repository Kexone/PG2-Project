#ifndef __PARTICLE_H_
#define __PARTICLE_H

#include <sceneInitializer.h>

struct Particle
{
	glm::vec3 Position;
	glm::vec3 Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(1.0f) { }
};

#endif