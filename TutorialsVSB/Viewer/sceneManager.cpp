////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	sceneManager.cpp
//
// summary:	Implements the scene manager class
// author:	Petr Gajdoš
// 
// Copyright © 2014 Petr Gajdoš. All Rights Reserved.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "sceneManager.h"
#include "sceneGUI.h"
#include "demos.h"

#pragma region INITIALIZATION OF STATICS

SceneManager *SceneManager::SINGLETON_INSTANCE = 0;
SingletonDestroyer<SceneManager> SceneManager::destroyer;

SceneSetting *SceneSetting::SINGLETON_INSTANCE = 0;
SingletonDestroyer<SceneSetting> SceneSetting::destroyer;

SceneData *SceneData::SINGLETON_INSTANCE = 0;
SingletonDestroyer<SceneData> SceneData::destroyer;

GUIManager *GUIManager::SINGLETON_INSTANCE = 0;
SingletonDestroyer<GUIManager> GUIManager::destroyer;

FT_Library Font::ftLibrary;
bool Font::ftLoaded = false;

char Light::tmp[128] = { '\0' };

#pragma endregion

void SceneManager::updateCameraPerspective(Camera *c, bool firstInit)
{
	if (firstInit)		//Decide what camera you need
	{
		//left, right, bottom, top is set relatively to the camera target !!!
		//c->ortho(-2.0f,2.0f,-2.0f,2.0f, 0.01f, 100.0f);
		//c->setBehavior(Camera::CAMERA_BEHAVIOR_FIRST_PERSON);
		//c->setPosition(glm::vec3(1.0f, 1.0f, 2.0f));
		//c->lookAt(glm::vec3(1.0f, 1.0f, 0.0f));

		c->perspective(90.f, (float)m_sceneSetting->m_screen[0] / m_sceneSetting->m_screen[1], 0.01f, 200.0f);
		c->setBehavior(Camera::CAMERA_BEHAVIOR_ORBIT);
		c->setOrbitMinZoom(0.1f);
		c->setOrbitMaxZoom(100);
		c->setOrbitOffsetDistance(2.0f);
		c->lookAt(glm::vec3(0.5f, 1.0f, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,0.0f,1.0f));
	}
	else
	{
		if (c->isPerspectiveCamera())
		{
			c->perspective(90.f, (float)m_sceneSetting->m_screen[0] / m_sceneSetting->m_screen[1], 0.01f, 100.0f);
		}
	}
}

SceneManager::SceneManager() : ThreadObject()
{
	m_sceneData = SceneData::GetInstance();
	m_guiManager = GUIManager::getInstance();
	m_guiManager->setSceneData(m_sceneData);

	//Set current scene setting
	m_sceneSetting = SceneSetting::GetInstance();
	m_sceneSetting->m_screen[0] = m_sceneSetting->m_screen[1] = 1024;
	m_sceneSetting->m_clearColor[0] = m_sceneSetting->m_clearColor[1] = m_sceneSetting->m_clearColor[2] = 0.95f;
	m_sceneSetting->m_clearColor[3] = 1.0f;
	m_sceneSetting->m_drawingMode = e_DrawingMode::SOLID;

	m_isRunning = true;
	m_fpsCounter = 0;
}

SceneManager::~SceneManager() 
{	
	FreeImage_DeInitialise();
}

SceneManager* SceneManager::GetInstance() 
{
	if (SINGLETON_INSTANCE == NULL)  // is it the first call?
	{  
		SINGLETON_INSTANCE = new SceneManager(); // create sole instance
		destroyer.SetDoomed(SINGLETON_INSTANCE); 
	}

	return SINGLETON_INSTANCE;
}

void SceneManager::Close()
{
	m_isRunning = false;
	SAFE_DELETE(m_sceneInitializer);
	m_guiManager->free();
}

//-----------------------------------------------------------------------------------------------------------------
//OpenGL Initialization
//-----------------------------------------------------------------------------------------------------------------
void SceneManager::Init(int argc, char* argv[]) 
{
	glutInit(&argc, argv);

	//glutInitContextVersion(4, 4);
	glutInitContextProfile(GLUT_CORE_PROFILE);						//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(m_sceneSetting->m_screen[0], m_sceneSetting->m_screen[1]);
	glutInitWindowPosition(0, 0);

	m_mainWindow = glutCreateWindow(0);
	sprintf_s(m_windowsTitle, 512, "SimpleView | context %s | renderer %s | vendor %s ",
			(const char*)glGetString(GL_VERSION),
			(const char*)glGetString(GL_RENDERER),
			(const char*)glGetString(GL_VENDOR));
	glutSetWindowTitle(m_windowsTitle);

	glewExperimental=TRUE;
	if (GLEW_OK != glewInit())
	{
		exit(1);
	}
	if (!glewIsSupported("GL_VERSION_3_3 "))
	{
		printf("ERROR: Support for necessary OpenGL extensions missing.");
		return;
	} 

	FreeImage_Initialise();
	m_guiManager->init();

	//glutFullScreen();

	//glutSetOption is only available with freeglut !!!
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_EXIT);

	//Close function
	glutCloseFunc(SceneManager::CloseCB);

	// Display and Idle
	glutDisplayFunc(SceneManager::DisplayCB);
	glutReshapeFunc(SceneManager::ReshapeCB);
	glutTimerFunc(250, TimeFuncCB, 1);
	glutIdleFunc(SceneManager::IdleCB);

	// Keyboard
	glutKeyboardFunc(SceneManager::KeyTypedCB);
	glutSpecialFunc(SceneManager::InputKeyCB);

	// Mouse stuff
	glutMouseFunc(Mouse::clickCB);
	glutMotionFunc(Mouse::activeMoveCB);
	glutPassiveMotionFunc(Mouse::passiveMoveCB);
	glutMouseWheelFunc(Mouse::wheelCB);

	//sceneInitializer = new Demo8(sceneData);
	m_sceneInitializer = new LOAD_DEMO;
	m_sceneInitializer->init();

	m_sceneSetting->m_activeCamera = m_sceneData->cameras[0];
	updateCameraPerspective(m_sceneSetting->m_activeCamera, true);

	m_guiManager->addBar(SceneGUI::createBar()); 

	Mouse *m = new Mouse(m_sceneSetting->m_activeCamera);
	m_sceneData->mouse = m;
	Mouse::CALLBACK_INSTANCE = m;

	glClearColor(m_sceneSetting->m_clearColor[0], m_sceneSetting->m_clearColor[1], m_sceneSetting->m_clearColor[2], m_sceneSetting->m_clearColor[3]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

//-----------------------------------------------------------------------------------------------------------------
//OpenGL Display and Idle functions
//-----------------------------------------------------------------------------------------------------------------
void SceneManager::Display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	m_sceneInitializer->render();
	
	m_guiManager->display();

	glFlush();
	glutSwapBuffers();
	m_fpsCounter++;
}

void SceneManager::Redisplay() 
{
	glutSetWindow(m_mainWindow);
	glutPostRedisplay();
}

void SceneManager::TimeFunc(int value)
{
	if (value != 0)
	{
		size_t length = 512 + strlen(m_windowsTitle);
		char* TempString = (char*)malloc(length);
 
		sprintf_s(TempString, length, "%s: %d FPS @ %d x %d",
			m_windowsTitle,
			m_fpsCounter * 4,
			m_sceneSetting->m_screen[0],
			m_sceneSetting->m_screen[1]
		);
 
		glutSetWindowTitle(TempString);
		free(TempString);
	}
    m_fpsCounter = 0;
    glutTimerFunc(250, TimeFuncCB, 1);
}

void SceneManager::Idle() 
{
	if (m_sceneData->idleEntities.size() > 0)
	{
		for (unsigned int i=0; i<m_sceneData->idleEntities.size(); i++)		//Volaji se vsechny ukazatele na idle funkce.
		{
			m_sceneData->idleEntities[i]->idle();						
		}
	}
	//glutPostRedisplay();
}

//-----------------------------------------------------------------------------------------------------------------
//OpenGL Reshape functions
//-----------------------------------------------------------------------------------------------------------------
void SceneManager::Reshape(int width, int height) 
{
	float aspectRatio = 1.33f;
	height            = height <= 0 ? 1 : height;
	aspectRatio       = (float)width/height;

	m_sceneSetting->m_screen[0] = width;
	m_sceneSetting->m_screen[1] = height;

	//if (m_sceneData->renderer)
	//	m_sceneData->renderer->init(m_sceneData);

	updateCameraPerspective(m_sceneSetting->m_activeCamera, false);

	glViewport(0, 0, width, height);
	
	m_guiManager->reshape();

	for (unsigned int i = 0; i < m_sceneData->fbos.size(); i++)
	{
		m_sceneData->fbos[i]->resize();
	}


	Display();
}

//-----------------------------------------------------------------------------------------------------------------
//OpenGL Input functions
//-----------------------------------------------------------------------------------------------------------------
void SceneManager::KeyTyped(unsigned char key, int x, int y)
{
	if (TwEventKeyboardGLUT(key, x, y)) { return; }

	if (key == 27)
		exit(0);

	else if (key == 'w')
	{
		int i[4] = {0,0,0,0};
		glGetIntegerv(GL_POLYGON_MODE, i);
		if (i[0] == GL_LINE)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	else if (key == 'c')
	{
		if (glIsEnabled(GL_CULL_FACE))
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);
	}

}

void SceneManager::InputKey(int key, int x, int y)
{
}

//-----------------------------------------------------------------------------------------------------------------
//UTILS functions
//-----------------------------------------------------------------------------------------------------------------
void SceneManager::addEntity(Entity *e, const unsigned int shaderProgramID)
{
	m_sceneData->sceneEntities.push_back(e);
	//sceneData->idleEntities.push_back(e);
	m_sceneData->nonInitializedEntities.push_back(e);
	glutPostRedisplay();
}

void SceneManager::start()
{
	while(m_isRunning)
	{
		glutMainLoopEvent();
		if (m_sceneData->nonInitializedEntities.size()>0)
		{
			for (unsigned int i=0; i<m_sceneData->nonInitializedEntities.size(); i++)
			{
				m_sceneData->nonInitializedEntities.at(i)->init();
			}
			m_sceneData->nonInitializedEntities.clear();
			pthread_cond_broadcast(&(mt->condition));
			pthread_mutex_unlock(&(mt->mutex));
		}
		glutPostRedisplay();
	}
}

void SceneManager::stop()
{
	m_isRunning = false;
}


