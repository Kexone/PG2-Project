#include "demo6_1.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_OBJ.h"
#include "entity_CubeV3C4.h"
void Demo6_1::initShaders()
{
	
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag", 0);
	initShaderProgram("adsOBJ_v3_n3_t3.vert", "adsOBJ_v3_n3_t3.frag", 0);
	initShaderProgram("goochOBJ_v3_n3_t3.vert", "goochOBJ_v3_n3_t3.frag", 0);
	//TODO: Add shaders
	//e.g. initShaderProgram("adsOBJ_v3_n3_t3.vert", "adsOBJ_v3_n3_t3.frag", 0);
	resetResPath();
}

void Demo6_1::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("vase/vase.obj"));
	m_sceneData->models.push_back(m);

	resetResPath();
}

void Demo6_1::initVAOs()
{
	VAO_SceneOrigin* vao0 = new VAO_SceneOrigin();
	vao0->init();
	m_sceneData->vaos.push_back(vao0);

	VAO_GridXY* vao1 = new VAO_GridXY();
	vao1->init();
	m_sceneData->vaos.push_back(vao1);

	VAO* vao2 = new VAO();
	vao2->createFromModel(m_sceneData->models[0]);
	m_sceneData->vaos.push_back(vao2);
}

void Demo6_1::initTextures()
{
	addResPath("textures/");
	//Load sprite textures
	GLuint texID;
	FIBITMAP *image = ImageManager::GenericLoader(getResFile("skala.bmp"), 0);

	//TODO Create Texture:
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 1.5f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	FreeImage_Unload(image);
	m_sceneData->textures.push_back(texID);

	resetResPath();
}

void Demo6_1::initMaterials()
{
	Material *m = new Material();
	m->setName("White_opaque");
	m->m_ambient[0] = m->m_ambient[1] = m->m_ambient[2] = 1.0f;	 m->m_ambient[3] = 1.0f;
	m->m_diffuse[0] = 0.8f; m->m_diffuse[1] = 1.0; m->m_diffuse[2] = 0.8f;	 m->m_diffuse[3] = 1.0f;
	m->m_specular[0] = m->m_specular[1] = m->m_specular[2] = 0.2f; m->m_specular[3] = 1.0f;
	m->m_transparency = 0.0f;
	m_sceneData->materials.push_back(m);
}

void Demo6_1::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void Demo6_1::initSceneEntities()
{
	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj->setPosition(0, -1.5f, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj->setPosition(0, 1.5f, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);

	obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj->setPosition(0, 0, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);
}

void Demo6_1::render()
{
	Entity *e = nullptr;
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


	//TODO: Uncomment, to change the active shader 
	//ss->m_activeShader = m_sceneData->shaderPrograms[2];
	//ss->m_activeShader->enable();
	ss->m_activeShader = m_sceneData->shaderPrograms[2];
	ss->m_activeShader->enable();
	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");

	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());

	e = m_sceneData->sceneEntities[2];
	Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");
	e->draw();


	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	//glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

	ss->m_activeShader = m_sceneData->shaderPrograms[1];
	ss->m_activeShader->enable();


	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");

	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());

	e = m_sceneData->sceneEntities[0];
	Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");
	e->draw();

	e = m_sceneData->sceneEntities[1];
	Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");
	e->draw();

	


	
	//copy imgs
	unsigned char buffer[3 * width * height];
	glReadBuffer(GL_FRONT);
	glReadPixels(20, 20, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	glRasterPos3f(0.0, 0.0, 0.0);
	glDrawBuffer(GL_BACK);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer);


	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

#pragma endregion

}
