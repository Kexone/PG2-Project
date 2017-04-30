#pragma once
#include <glew.h>
#include <detail/type_vec2.hpp>
#include <vector>

struct Particle;

class ParticleSystem
{
public:
	GLuint FirstUnusedParticle(GLuint nr_particles, std::vector<Particle> particles);
	void RespawnParticle(Particle &particle, glm::vec3 offset);
private:
	GLuint lastUsedParticle = 0;
};
