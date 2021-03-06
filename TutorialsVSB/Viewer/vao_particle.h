#ifndef __VAO_PARTICLE_H_
#define __VAO_PARTICLE_H_

#include "vao.h"

class VAO_Particle : public VAO
{
public:
	VAO_Particle() : VAO() {}
	virtual ~VAO_Particle(void) {}

	virtual void init()
	{
		GLfloat particle_quad[] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		GLuint tmpBuffer;
		glGenVertexArrays(1, &m_object);
		glGenBuffers(1, &tmpBuffer);
		glBindVertexArray(m_object);
		// Fill mesh buffer
		glBindBuffer(GL_ARRAY_BUFFER, tmpBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
		// Set mesh attributes
		glEnableVertexAttribArray(0); // position
		glEnableVertexAttribArray(1); // uv coords
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
		glBindVertexArray(0);

		////Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_isInitialized = true;
	}
};

#endif