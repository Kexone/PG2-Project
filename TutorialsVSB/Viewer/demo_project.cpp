#include "demo_Project.h"
#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_particle.h"
#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_OBJ.h"
#include "entity_Particles.h"
#include <vector>

void DemoProject::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");
	initShaderProgram("project/goochOBJ_v3_n3_t3.vert", "project/goochOBJ_v3_n3_t3.frag");
	initShaderProgram("project/adsOBJ_v3_n3_t3_phong.vert", "project/adsOBJ_v3_n3_t3_phong.frag");
	initShaderProgram("project/adsOBJ_v3_n3_t3_normal.vert", "project/adsOBJ_v3_n3_t3_normal.frag");
	initShaderProgram("project/adsOBJ_v3_n3_t3_parallax_steep_interpol.vert", "project/adsOBJ_v3_n3_t3_parallax_steep_interpol.frag");
	initShaderProgram("project/adsObj_v3_n3_t3_displacement.vert", "project/adsObj_v3_n3_t3_displacement.frag", 0, "project/adsObj_v3_n3_t3_displacement.cont", "project/adsObj_v3_n3_t3_displacement.eval");
	initShaderProgram("project/particles_render.vert", "project/particles_render.frag");
	resetResPath();
}

void DemoProject::initModels()
{
	position = 5;
	changeDir = false;
	const char* models[] =
	{
		"basic/cube.obj",
		"basic/plane.obj",
		"project/ChestCartoon/ChestCartoon.obj",
		"basic/bigplane.obj",
		"project/coin.obj",
		"project/portal/portals.obj",
		"project/arrow.obj",
	};
	ObjLoader objL;
	Model* m;
	addResPath("models/");
	for (int i = 0; i < 7; i++)
	{
		m = objL.loadModel(getResFile(models[i]));
		m_sceneData->models.push_back(m);
	}
	resetResPath();
}


void DemoProject::initVAOs()
{
	VAO_SceneOrigin* vao0 = new VAO_SceneOrigin();
	vao0->init();
	m_sceneData->vaos.push_back(vao0);

	VAO_GridXY* vao1 = new VAO_GridXY();
	vao1->init();
	m_sceneData->vaos.push_back(vao1);

	VAO* vao2 = new VAO();
	vao2->createFromModelWithTBN(m_sceneData->models[0]);
	m_sceneData->vaos.push_back(vao2);

	//plane
	VAO* vao3 = new VAO();
	vao3->createFromModelWithTBN(m_sceneData->models[1]);
	m_sceneData->vaos.push_back(vao3);

	//chest
	VAO* vao4 = new VAO();
	vao4->createFromModelWithTBN(m_sceneData->models[2]);
	m_sceneData->vaos.push_back(vao4);

	//bigplane
	VAO* vao5 = new VAO();
	vao5->createFromModelWithTBN(m_sceneData->models[3]);
	m_sceneData->vaos.push_back(vao5);

	//coin
	VAO* vao6 = new VAO();
	vao6->createFromModelWithTBN(m_sceneData->models[4]);
	m_sceneData->vaos.push_back(vao6);

	VAO_Particle* vaoPortal = new VAO_Particle();
	vaoPortal->init();
	m_sceneData->vaos.push_back(vaoPortal);

	VAO* vao7 = new VAO();
	vao7->createFromModelWithTBN(m_sceneData->models[5]);
	m_sceneData->vaos.push_back(vao7);

	VAO_Particle* vaoParticles = new VAO_Particle();
	vaoParticles->init();
	m_sceneData->vaos.push_back(vaoParticles);

	VAO* vao9 = new VAO();
	vao9->createFromModelWithTBN(m_sceneData->models[6]);
	m_sceneData->vaos.push_back(vao9);
}

void DemoProject::initTextures()
{
	addResPath("textures/");
	//Load sprite textures
	GLuint texID;
	const char *texture[] = { 
		"grassDIFFUSE.jpg",
		"grassNORMAL.jpg",
		"grassDISP.jpg",
		"wood_d.jpg",
		"wood_n.jpg",
		"wood_h.jpg",
		"sand.jpg",
		
	};
	for(int i = 0; i < 7; i++)
	{
		FIBITMAP *image = ImageManager::GenericLoader(getResFile(texture[i]), 0);
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Load tex image based on depth
		if (FreeImage_GetBPP(image) == 8) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_RED, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
		}
		else if (FreeImage_GetBPP(image) == 24) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
		}
		else if (FreeImage_GetBPP(image) == 32) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
		glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 1.5f);

		FreeImage_Unload(image);
		m_sceneData->textures.push_back(texID);
	}
	resetResPath();
}

void DemoProject::initMaterials()
{
	Material *m = new Material();

	m->setName("White_opaque");
	m->m_diffuse[0] = 1.0f;
	m->m_diffuse[1] = 1.0f;
	m->m_diffuse[2] = 1.0f;
	m->m_diffuse[3] = 1.0f;
	m->m_transparency = 0.0f;
	m->m_specular[0] = 1.0f;
	m->m_specular[1] = 1.0f;
	m->m_specular[2] = 1.0f;
	m->m_specular[3] = 1.0f;
	m->m_ambient[0] = 1.0f;
	m->m_ambient[1] = 1.0f;
	m->m_ambient[2] = 1.0f;
	m->m_ambient[3] = 1.0f;

	m->m_diffuseTextureGL = m_sceneData->textures[0];
	m_sceneData->materials.push_back(m);

	m = new Material();
	m->setName("Gold");
	m->m_diffuse[0] = 1.0f;
	m->m_diffuse[1] = 0.68f;
	m->m_diffuse[2] = -0.23f;
	m->m_diffuse[3] = 0.12f;
	m->m_transparency = 0.0f;
	m->m_specular[0] = 1.0f;
	m->m_specular[1] = 1.0f;
	m->m_specular[2] = 1.0f;
	m->m_specular[3] = 1.0f;
	m->m_ambient[0] = 1.0f;
	m->m_ambient[1] = 1.0f;
	m->m_ambient[2] = 1.0f;
	m->m_ambient[3] = 1.0f;

	m->m_diffuseTextureGL = m_sceneData->textures[1];
	m_sceneData->materials.push_back(m);
}

void DemoProject::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void DemoProject::initSceneEntities()
{
	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj->setPosition(4, 4, 2);
	obj->setOrientation(0, 0, 180);
	obj->setScale(0.5, 0.5, 0.5);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	obj = new Entity_OBJ(m_sceneData->models[1], m_sceneData->vaos[3]);
	obj->setPosition(2, 4, 6);
	obj->setOrientation(0, 0, 0);
	obj->setScale(1, 1, 1);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	//chest
	obj = new Entity_OBJ(m_sceneData->models[2], m_sceneData->vaos[4]);
	obj->setPosition(-2, 1, 0.9);
	obj->setOrientation(0, 0, 180);
	obj->setScale(0.5, 0.5, 0.5);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	
	obj = new Entity_OBJ(m_sceneData->models[3], m_sceneData->vaos[5]);
	obj->setPosition(1, 1, 1);
	obj->setOrientation(0, 0, 0);
	obj->setScale(1, 1, 1);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);
	
	//coin
	obj = new Entity_OBJ(m_sceneData->models[4], m_sceneData->vaos[6]);
	obj->setPosition(4, 1, 4);
	obj->setOrientation(180, 180, 270);
	obj->m_material = m_sceneData->materials[1];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	//particle portal
	Entity_Particles* particle = new Entity_Particles(0,m_sceneData->vaos[7]);
	particle->setPosition(0, 0, 0);
	particle->m_material = m_sceneData->materials[1];
	particle->init();
	m_sceneData->sceneEntities.push_back(particle);

	//portal door
	obj = new Entity_OBJ(m_sceneData->models[5], m_sceneData->vaos[8]);
	obj->setPosition(6.3, 8.3, 1);
	obj->setScale(3.5, 3.5, 2.22);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	//particle
	particle = new Entity_Particles(1,m_sceneData->vaos[9]);
	particle->setPosition(-2.5, 0.2, 1.5);
	particle->setScale(0.5, 0.5, 0.5);
	particle->m_material = m_sceneData->materials[1];
	particle->init();
	m_sceneData->sceneEntities.push_back(particle);

	//arrow
	obj = new Entity_OBJ(m_sceneData->models[6], m_sceneData->vaos[10]);
	obj->setPosition(-2.5, 1, 5);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);
}

void DemoProject::render()
{
	Entity_OBJ *e = nullptr;
	SceneSetting *ss = SceneSetting::GetInstance();

#pragma region Draw Info Entities

	ss->m_activeShader = m_sceneData->shaderPrograms[0];
	ss->m_activeShader->enable();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	int uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVPMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewProjectionMatrix());
	for (unsigned int i = 0; i < m_sceneData->infoEntities.size(); i++) 
		m_sceneData->infoEntities[i]->draw();

#pragma endregion

#pragma region Draw Scene Entities

	#pragma region Draw Cube
		ss->m_activeShader = m_sceneData->shaderPrograms[4];
		ss->m_activeShader->enable();
		e = static_cast<Entity_OBJ*>(m_sceneData->sceneEntities[0]);

		Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");

		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());

		Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");

		// Projection matrix
		glm::mat4 projectionMatrix({
			1.0f, 0, 0, 0,
			0, 1.0f, 0, 0,
			0, 0, 1.0f, 0,
			0, 0, 0, 1.0f,
		});

		// Copy matrix to vertex shader
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "TMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, (float*)&projectionMatrix[0]);

		// Activate textures & copy textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[0]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "diffuseTexture");
		glUniform1i(uniform, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[1]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "normalTexture");
		glUniform1i(uniform, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[2]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "depthTexture");
		glUniform1i(uniform, 2);
		e->setPosition(0, 0, 0);
		e->rotate(0.1, 0, 0, 1);	
		e->setPosition(4, -4, 2);
		//e->rotate(0.1, 1, 0, 0);
		//e->setScale(1, 1, 1);
		//e->setPosition(1, -1, 1);
		e->draw();

	#pragma endregion 

	//#pragma region Draw Plane test
	//	e = static_cast<Entity_OBJ*>(m_sceneData->sceneEntities[1]);
	//	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	//	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	//	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	//	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
	//	e->draw();
	//#pragma endregion 


	#pragma region Draw chest
		ss->m_activeShader = m_sceneData->shaderPrograms[4];
		ss->m_activeShader->enable();
		Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");
		Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");
		e = static_cast<Entity_OBJ*>(m_sceneData->sceneEntities[2]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[3]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "diffuseTexture");
		glUniform1i(uniform, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[4]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "normalTexture");
		glUniform1i(uniform, 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[5]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "depthTexture");
		glUniform1i(uniform, 2);

		e->draw();
	#pragma endregion 

	#pragma region Draw main plane
		ss->m_activeShader = m_sceneData->shaderPrograms[4];
		ss->m_activeShader->enable();
		e = static_cast<Entity_OBJ*>(m_sceneData->sceneEntities[3]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[0]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "diffuseTexture");
		glUniform1i(uniform, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[1]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "normalTexture");
		glUniform1i(uniform, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[2]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "depthTexture");
		e->draw();
	#pragma endregion
	
	#pragma region Draw coins
		ss->m_activeShader = m_sceneData->shaderPrograms[2];
		ss->m_activeShader->enable();
		e = static_cast<Entity_OBJ*>(m_sceneData->sceneEntities[4]);
		float positionCoinY[] = { 0.7, 1.2, 2.2, 3.7, 4.2 };
		float positionCoinX[] = { -1.1, -1.5, -2.0, -2.2, -2.7 };
		float tiltRoll[] = { 45, 20 };
		for (int h = 1; h < 5; h++) {
			for (int i = 0; i < 5; i++)
			{
				Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");
				Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");
				uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
				glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
				uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
				glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
				for (int j = 0; j < 2; j++)
				{
					e->setPosition(positionCoinX[i], positionCoinY[j], h / 2);
					e->setOrientation(tiltRoll[j], 0, positionCoinX[i] * 100);
					e->draw();
				}
			}
		}
	#pragma endregion

	#pragma region Draw portal door
		ss->m_activeShader = m_sceneData->shaderPrograms[2];
		ss->m_activeShader->enable();
		e = static_cast<Entity_OBJ*>(m_sceneData->sceneEntities[6]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[6]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "diffuseTexture");
		glUniform1i(uniform, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[1]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "normalTexture");
		glUniform1i(uniform, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_sceneData->textures[2]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "depthTexture");
		e->draw();
	#pragma endregion

	#pragma region Draw portal particles
		ss->m_activeShader = m_sceneData->shaderPrograms[6];
		
		Entity_Particles* particles = (Entity_Particles*)m_sceneData->sceneEntities[5];
		particles->setScale(10.1, 10, 10);
		particles->setPosition(2, 2.5, 4.8);
		particles->update();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		ss->m_activeShader->enable();

		//Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");
		particles->draw();
		ss->m_activeShader->disable();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	#pragma endregion


	#pragma region Draw particles
		ss->m_activeShader = m_sceneData->shaderPrograms[6];

		particles = (Entity_Particles*)m_sceneData->sceneEntities[7];
		
		particles->update();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		ss->m_activeShader->enable();
		Material::setShaderUniform(particles->m_material, ss->m_activeShader, "material");
		particles->draw();
		ss->m_activeShader->disable();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	#pragma endregion

	#pragma region Draw arrow
		ss->m_activeShader = m_sceneData->shaderPrograms[1];
		ss->m_activeShader->enable();
		e = static_cast<Entity_OBJ*>(m_sceneData->sceneEntities[8]);
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
		uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());
		e->setPosition(-2, 1, position);
		e->draw();
	if(!changeDir)
	{
		position -= 0.05;

	}
	else if(changeDir)
	{
		position += 0.05;
	}
	if (position > 5.5)
	{
		changeDir = false;
	}
	if (position < 4.0)
	{
		changeDir = true;
	}
	#pragma endregion
#pragma endregion
}