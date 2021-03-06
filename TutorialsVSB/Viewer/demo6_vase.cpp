#include "Demo6_vase.h"

#include "vao_SceneOrigin.h"
#include "vao_GridXY.h"
#include "vao_CubeV3C4N3T2.h"

#include "entity_SceneOrigin.h"
#include "entity_GridXY.h"
#include "entity_Cube.h"
#include "entity_OBJ.h"

void Demo6Vase::initShaders()
{
	addResPath("shaders/");
	initShaderProgram("simple_v3_c4.vert", "simple_v3_c4.frag");

	//TODO - update previous ADS shader to accept texture:
	initShaderProgram("adsOBJ_v3_n3_t3.vert", "adsOBJ_v3_n3_t3.frag");
	resetResPath();
}

void Demo6Vase::initModels()
{
	ObjLoader objL;
	Model* m;

	addResPath("models/");

	m = objL.loadModel(getResFile("vase/vase.obj"));
	m_sceneData->models.push_back(m);

	resetResPath();
}

void Demo6Vase::initVAOs()
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

void Demo6Vase::initTextures()
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Give the image to OpenGL
	if (FreeImage_GetBPP(image) == 8)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_RED, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
	else if (FreeImage_GetBPP(image) == 24)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGR, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));
	else if (FreeImage_GetBPP(image) == 32)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image), FreeImage_GetHeight(image), 0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(image));



	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 1.5f);

	FreeImage_Unload(image);
	m_sceneData->textures.push_back(texID);

	resetResPath();
}

void Demo6Vase::initMaterials()
{
	Material *m = new Material();

	m->setName("White_opaque");
	m->m_diffuse[0] = 0.0f;
	m->m_diffuse[1] = 0.0f;
	m->m_diffuse[2] = 0.0f;
	m->m_diffuse[3] = 1.0f;
	m->m_transparency = 0.0f;

	m->m_diffuseTextureGL = m_sceneData->textures[0];
	m_sceneData->materials.push_back(m);
}

void Demo6Vase::initInfoEntities()
{
	Entity_SceneOrigin *e0 = new Entity_SceneOrigin(m_sceneData->vaos[0]);
	e0->init();
	m_sceneData->infoEntities.push_back(e0);

	Entity_GridXY *e1 = new Entity_GridXY(m_sceneData->vaos[1]);
	e1->init();
	m_sceneData->infoEntities.push_back(e1);
}

void Demo6Vase::initSceneEntities()
{
	Entity_OBJ *obj = new Entity_OBJ(m_sceneData->models[0], m_sceneData->vaos[2]);
	obj->setPosition(0, -1.5f, 0);
	obj->setOrientation(0, 0, 90);
	obj->m_material = m_sceneData->materials[0];
	obj->init();
	m_sceneData->sceneEntities.push_back(obj);
}

void Demo6Vase::render()
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

	ss->m_activeShader = m_sceneData->shaderPrograms[1];
	ss->m_activeShader->enable();

	Light::setShaderUniform(m_sceneData->lights.at(0), ss->m_activeShader, "light");

	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "PMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getProjectionMatrix());
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "VMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, ss->m_activeCamera->getViewMatrix());

	e = m_sceneData->sceneEntities[0];

	glm::mat4 tmp = ss->m_activeCamera->getVM() * e->m_modelMatrix;
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "MVMatrix");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, (float*)&tmp[0]);
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "NormalMatrix");
	glUniformMatrix3fv(uniform, 1, GL_FALSE, (float*)&(glm::inverseTranspose(glm::mat3(tmp)))[0]);

	Material::setShaderUniform(e->m_material, ss->m_activeShader, "material");

	//TODO: Active Texture Unit and Bind Texture

	glActiveTexture(GL_TEXTURE0);
	uniform = glGetUniformLocation(ss->m_activeShader->m_programObject, "Texture");
	glUniform1i(uniform, 0);
	e->draw();

#pragma endregion

}
