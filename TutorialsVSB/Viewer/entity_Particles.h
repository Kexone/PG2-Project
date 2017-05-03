#ifndef __Entity_Particles_H_
#define __Entity_Particles_H_

#include "stdafx.h"

#include <glm.hpp>
#include "entity_VAO.h"
#include "vao_particle.h"
#include <vector>
#include "Particle.h"


# define M_PI           3.14159265358979323846  /* pi */


class Entity_Particles : public Entity_VAO
{

public:
	Entity_Particles(int type, VAO* vao) : Entity_VAO(vao) {
		vao_particle = (VAO_Particle*)vao;
		Type = type;
		if (Type == 1)
			nr_particles = 1000;
		// Create this->amount default particle instances
		for (GLuint i = 0; i < nr_particles; ++i)
			this->particles.push_back(Particle());

	}
	virtual ~Entity_Particles(void) {}

	virtual void init();
	virtual void update();
	virtual void draw(const unsigned int eid = 0);

	GLuint FirstUnusedParticle();
	void RespawnParticle(Particle &particle, glm::vec3 velocity, glm::vec3 offset);
protected:
	GLuint nr_particles = 500;
	GLuint lastUsedParticle = 0;
	vector<Particle> particles;
	VAO_Particle* vao_particle;
	int Type;

};

inline void Entity_Particles::init()
{
	if ((m_vao) && (vao_particle))
	{	
		m_isInitialized = true;
	}
}

GLuint Entity_Particles::FirstUnusedParticle()
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
	//lastUsedParticle = 0;
	return lastUsedParticle;
}

void Entity_Particles::RespawnParticle(Particle &particle, glm::vec3 velocity, glm::vec3 offset)
{

	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5f + ((rand() % 100) / 100.0f);
	GLfloat bColor = 0.5f + ((rand() % 100) / 100.0f);
	GLfloat gColor = 0.5f + ((rand() % 100) / 100.0f);
	if (Type == 1)
	{
		particle.Position = offset;
		particle.Color = glm::vec3(1, 1, 0.1f);
	}
	if( Type == 0)
	{
		particle.Position = vec3(0.4, 0.6, 0);
		particle.Color = glm::vec3(rColor, rColor, gColor);
	}
	particle.Life = 1.0f;
	particle.Velocity = velocity;
}

float getRandom(int value)
{
	return (rand()  / value) * 0.1f ;
}

inline void Entity_Particles::update() {
	vec3 offset = vec3(0.0);
	GLuint nr_new_particles = 1;
	GLfloat dt = 0.01f;
	// Add new particles
	for (GLuint i = 0; i < nr_new_particles; ++i)
	{
		int unusedParticle = FirstUnusedParticle();
		if (unusedParticle >= 0) {
			if (Type == 0) {
				offset = vec3(getRandom(100), getRandom(100), getRandom(100) * 0.2f) * 0.5f;
				RespawnParticle(particles[unusedParticle], glm::vec3(0, 0.02, 0), offset);

			}
			else if(Type == 1) {
				offset = vec3(getRandom(200* M_PI ), getRandom(200 * M_PI), getRandom(200 * M_PI) ) * 0.5f;
				RespawnParticle(particles[unusedParticle], glm::vec3(0.0, 0.0, 1), offset);
			}
		}
	}
	// Uupdate all particles
	for (GLuint i = 0; i < nr_particles; ++i)
	{
		Particle &p = particles[i];
		p.Life -= dt; // reduce life
		if (p.Life > 0.0f)
		{	// particle is alive, thus update
			p.Position += p.Velocity * dt;
		}
	}
}

inline void Entity_Particles::draw(const unsigned int eid)
{
	if (!m_isInitialized) return;

	SceneSetting *ss = SceneSetting::GetInstance();
	glm::mat4 mvMatrix = ss->m_activeCamera->getVM() * m_modelMatrix;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// transparent
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);


	for (Particle particle : particles)
	{
		if (particle.Life > 0.0f)
		{
			int uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
			glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
			uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
			glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
			uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVMatrix");
			glUniformMatrix4fv(uniform, 1, GL_FALSE, (float*)&mvMatrix[0]);
			uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "offset");
			glUniform3fv(uniform, 1, (float*)&particle.Position[0]);
			uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "life");
			glUniform1fv(uniform, 1, &particle.Life);
			uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "color");
			glUniform3f(uniform, particle.Color.r, particle.Color.g, particle.Color.b);

			if ((uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "sprite")) >= 0) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_material->m_diffuseTextureGL);
				glUniform1i(uniform, 0);
			}

			glBindVertexArray(vao_particle->m_object);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);


}

#endif