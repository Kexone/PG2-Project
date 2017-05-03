#ifndef __PARTICLE_H_
#define __PARTICLE_H
#include <detail/type_vec3.hpp>


struct Particle
{
	glm::vec3 Position;
	glm::vec3 Velocity;
	glm::vec3 Color;
	GLfloat Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

#endif