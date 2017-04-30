#include "ParticleSystem.h"
#include "Particle.h"


GLuint ParticleSystem::FirstUnusedParticle(GLuint nr_particles, std::vector<Particle> particles)
{
	// Search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < nr_particles; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Override first particle if all others are alive
	lastUsedParticle = 0;
	return 0;
}

void ParticleSystem::RespawnParticle(Particle &particle, glm::vec3 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.Position = glm::vec3(2.0f)- random;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = glm::vec3(0.5, 0.5,0.5) * 0.1f * random;
}