////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	entity_Cube_Simple.h
//
// summary:	Declaration and implementation of Entity_Cube class
// author:	Petr Gajdoš
// 
// Copyright © 2014 Petr Gajdoš. All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ENTITY_CUBEV3C4_H_
#define ENTITY_CUBEV3C4_H_

#include "stdafx.h"
#include "entity_VAO.h"

class Entity_CubeV3C4 : public Entity_VAO
{
public:
	Entity_CubeV3C4(VAO* vao = nullptr) : Entity_VAO(vao) {}
	~Entity_CubeV3C4(void) {}

	void draw(const unsigned int eid = 0);
};

inline void Entity_CubeV3C4::draw(const unsigned int eid)
{
	if (!m_isInitialized) return;

	glBindVertexArray(m_vao->m_object);
	//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_QUADS, 0, 24);
	glBindVertexArray(0);
}

#endif