#include "game.h"

#include <iostream>
#include <time.h>
#include <gl.hpp>
#include <GLFW/glfw3.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <SOIL2\SOIL2.h>

#include "logger.h"
#include "shader.h"
#include "modelLoader.h"
#include "PhysicsController.h"
#include "QuatCamera.h"
#include "eventHandler.h"
#include "rocket.h"


GAME::Game::Game() : m_timeSinceLastUpdate(0), m_CurrentTime(0), m_PreviousTime(0), m_DrawDebugBool(false) {
	UTIL::LOG::Clear();
}


GAME::Game::~Game()
{
	


}

void GAME::Game::Init()
{
	srand(time(NULL));
	
	m_CameraController = m_standardCamera;

	//init camera
	m_window.resizeGL( WIN_WIDTH, WIN_HEIGHT);
	m_CameraController->SetAspectRatio(float(WIN_WIDTH / WIN_HEIGHT));
	m_rearViewCamera->SetAspectRatio(float(WIN_WIDTH / WIN_HEIGHT));
	m_firstPersonCamera->SetAspectRatio(float(WIN_WIDTH / WIN_HEIGHT));
	m_freeRoamCamera->SetAspectRatio(float(WIN_WIDTH / WIN_HEIGHT));
	m_standardCamera2->SetAspectRatio(float(WIN_WIDTH / WIN_HEIGHT));

	
	UTIL::LOG(UTIL::LOG::INFO) << "Loading Please Wait....";
	UTIL::LOG(UTIL::LOG::INFO) << " ";
	m_menuShader->CompileAndLinkShader("./external/assets/shaders/menu.vs", "./external/assets/shaders/menu.fs");
	MainMenu(false);
	RenderMainMenu();
	//link shaders
	m_objShader->CompileAndLinkShader("./external/assets/shaders/object.vs", "./external/assets/shaders/object.fs");
	m_MapShader->CompileAndLinkShader("./external/assets/shaders/map.vs", "./external/assets/shaders/map.fs");
	m_debugDrawerShader->CompileAndLinkShader("./external/assets/shaders/line.vs", "./external/assets/shaders/line.fs");
	// physics engine
	PHYSICS::PhysicsController::GetPhysicsInstance();
	//Event Handler
	UTIL::EventHandler::getInstance();

	//set camera
	CurrentCamera();
	//car
	UTIL::LOG(UTIL::LOG::INFO) << "Loading Cars Please Wait....";
	UTIL::LOG(UTIL::LOG::INFO) << " ";
	m_player1Car = std::make_shared<RaceCar>();
	m_player1Car->Init();
	m_player2Car = std::make_shared<RaceCar>(true);
	

	UTIL::LOG(UTIL::LOG::INFO) << "Loading the Map Please Wait....";
	UTIL::LOG(UTIL::LOG::INFO) << " ";
	//track
	m_map = std::make_shared<Map>();
	m_map->Init();

	UTIL::EventHandler::getInstance().SetGameState(MAIN_MENU);
	UTIL::EventHandler::getInstance().setCamera(m_CameraController);
	UTIL::EventHandler::getInstance().setCamera(m_freeRoamCamera);
	UTIL::EventHandler::getInstance().setCar(m_player1Car);
	UTIL::EventHandler::getInstance().setPlayer2Car(m_player2Car);
	glfwSetScrollCallback(m_window.GetWindow(), &UTIL::EventHandler::getInstance().ScrollButtonCallBack);
	glfwSetMouseButtonCallback(m_window.GetWindow(), &UTIL::EventHandler::getInstance().MouseButtonCallback);
	glfwSetKeyCallback(m_window.GetWindow(), &UTIL::EventHandler::getInstance().KeyCallBack);
	glfwSetWindowSizeCallback(m_window.GetWindow(), &UTIL::EventHandler::getInstance().ResizeCallBack);
	UTIL::LOG(UTIL::LOG::INFO) << "Done!, enjoy the game!";
	UTIL::LOG(UTIL::LOG::INFO) << " ";
	if (UTIL::EventHandler::getInstance().GetGameState() == MAIN_MENU)
	{
		MainMenu(true);
	}
	RunGame();

}

void GAME::Game::RunGame()
{
	m_PreviousTime = glfwGetTime();
	double fpsTime = glfwGetTime();
	int nbFrames = 0;
	// ---------------------------------- MEMORY POOL TEST -------------------------------------------------------------
	//MemoryPoolTest();
	//main game loop

	while (!glfwWindowShouldClose(m_window.GetWindow()) && !glfwGetKey(m_window.GetWindow(), GLFW_KEY_ESCAPE)) 
	{
		m_CurrentTime = (double)glfwGetTime();
		nbFrames++;
		//FPS
		/*if (m_CurrentTime - fpsTime >= 1.0)
		{
		//	printf("%f ms/frame\n", double(1000.0/nbFrames));
			nbFrames = 0;
			fpsTime += 1.0;
		}*/

		double elaspedTime = m_CurrentTime - m_PreviousTime; //m_CurrentTime - m_PreviousTime;
		m_PreviousTime = m_CurrentTime;
		m_timeSinceLastUpdate += elaspedTime;
	
		glfwPollEvents();
		PollKeyEvents();
		CurrentCamera();
		
		m_DrawDebugBool = UTIL::EventHandler::getInstance().GetWireBool();

		while (m_timeSinceLastUpdate >= MS_PER_UPDATE) {
			Update(MS_PER_UPDATE);
			m_timeSinceLastUpdate -= MS_PER_UPDATE;
		}

		Render(m_timeSinceLastUpdate / MS_PER_UPDATE);
		
	}
}


void GAME::Game::Update(double deltaTime)
{
	
	
		if (UTIL::EventHandler::getInstance().GetCameraState() == FREE_ROAM)
			m_window.update(deltaTime, m_freeRoamCamera);

		m_player1Car->Update(deltaTime);
		if (UTIL::EventHandler::getInstance().GetGameState() == TWO_PLAYERS)
		{
			m_player2Car->Update(deltaTime);
			m_standardCamera2->Follow(deltaTime, m_player2Car->GetVelocity(), m_player2Car->GetMatrix());
		}
		m_map->Update(deltaTime);
		m_CameraController->Follow(deltaTime, m_player1Car->GetVelocity(), m_player1Car->GetMatrix());
		PHYSICS::PhysicsController::GetPhysicsInstance().StepSimulation(deltaTime);
}

void GAME::Game::Render(double Interpolate)
{
	
	
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	if (UTIL::EventHandler::getInstance().GetGameState() != MAIN_MENU)
	{
		gl::Enable(gl::DEPTH_TEST);
		glfwWindowHint(GLFW_RESIZABLE, true);


		if (UTIL::EventHandler::getInstance().GetGameState() == TWO_PLAYERS)
		{
			//render everything twice.
			m_window.ViewPort(0, (float)(UTIL::EventHandler::getInstance().GetWindowHeight() / 2.0f), (float)UTIL::EventHandler::getInstance().GetWindowWidth(), (float)(UTIL::EventHandler::getInstance().GetWindowHeight() / 2.0f));
			m_CameraController->SetAspectRatio((float)UTIL::EventHandler::getInstance().GetWindowWidth() / (float)UTIL::EventHandler::getInstance().GetWindowHeight());
			m_objShader->Use();
			m_setModel = glm::mat4(1.0f);
			SetViewMatricies(m_objShader, m_setModel);
			m_player1Car->Render(m_objShader);
			m_player2Car->Render(m_objShader);

			m_window.ViewPort(0, (float)(UTIL::EventHandler::getInstance().GetWindowHeight() / 2.0f), (float)UTIL::EventHandler::getInstance().GetWindowWidth(), (float)(UTIL::EventHandler::getInstance().GetWindowHeight() / 2.0f));
			m_MapShader->Use();
			m_setModel = glm::mat4(1.0f) * m_map->GetTrackMatrix();
			SetViewMatricies(m_MapShader, m_setModel);
			m_map->Render(m_MapShader);


			//player 2 rendering
			m_window.ViewPort(0, 0, (float)UTIL::EventHandler::getInstance().GetWindowWidth(), (float)(UTIL::EventHandler::getInstance().GetWindowHeight() / 2.0f));
			m_standardCamera2->SetAspectRatio((float)UTIL::EventHandler::getInstance().GetWindowWidth() / (float)UTIL::EventHandler::getInstance().GetWindowHeight());
			m_objShader->Use();
			m_setModel = glm::mat4(1.0f);
			m_objShader->SetUniform("model", m_setModel);
			m_objShader->SetUniform("view", m_standardCamera2->GetView());
			m_objShader->SetUniform("projection", m_standardCamera2->GetProjection());
			m_player2Car->Render(m_objShader);
			m_player1Car->Render(m_objShader);

			m_window.ViewPort(0, 0, (float)UTIL::EventHandler::getInstance().GetWindowWidth(), (float)(UTIL::EventHandler::getInstance().GetWindowHeight() / 2.0f));
			m_MapShader->Use();
			m_setModel = glm::mat4(1.0f) * m_map->GetTrackMatrix();
			m_MapShader->SetUniform("model", m_setModel);
			m_MapShader->SetUniform("view", m_standardCamera2->GetView());
			m_MapShader->SetUniform("projection", m_standardCamera2->GetProjection());
			m_map->Render(m_MapShader);
		}
		else if (UTIL::EventHandler::getInstance().GetGameState() == NORMAL || UTIL::EventHandler::getInstance().GetGameState() == WITHOUT_PATTERNS || UTIL::EventHandler::getInstance().GetGameState() == WITH_PATTERNS)
		{
			if (m_DrawDebugBool)
			{
				m_debugDrawerShader->Use();
				m_setModel = glm::mat4(1.0f);
				SetViewMatricies(m_debugDrawerShader, m_setModel);
				PHYSICS::PhysicsController::GetPhysicsInstance().DrawDebugWorld();
				WireFrameMode(PHYSICS::PhysicsController::GetPhysicsInstance().GetDebugDrawer()->GetLines());
			}
			else
			{
				m_window.ViewPort(0, 0, UTIL::EventHandler::getInstance().GetWindowWidth(), (UTIL::EventHandler::getInstance().GetWindowHeight()));
				m_objShader->Use();
				m_setModel = glm::mat4(1.0f);
				SetViewMatricies(m_objShader, m_setModel);
				m_player1Car->Render(m_objShader);

				m_window.ViewPort(0, 0, UTIL::EventHandler::getInstance().GetWindowWidth(), (UTIL::EventHandler::getInstance().GetWindowHeight()));
				m_MapShader->Use();
				m_setModel = glm::mat4(1.0f) * m_map->GetTrackMatrix();
				SetViewMatricies(m_MapShader, m_setModel);
				m_map->Render(m_MapShader);
			}
		}


		m_window.Display();
	}
	if (UTIL::EventHandler::getInstance().GetGameState() == MAIN_MENU)
	{
		RenderMainMenu();
	}
	
}

void GAME::Game::SetViewMatricies(std::shared_ptr<GRAPHICS::Shader> Shader, glm::mat4 model)
{

	Shader->SetUniform("model", model);

	if (UTIL::EventHandler::getInstance().GetCameraState() == FREE_ROAM)
	{
		Shader->SetUniform("view", m_freeRoamCamera->View());
		Shader->SetUniform("projection", m_freeRoamCamera->Projection());
	}
	else
	{
		Shader->SetUniform("view", m_CameraController->GetView());
		Shader->SetUniform("projection", m_CameraController->GetProjection());
	}


}

void GAME::Game::WireFrameMode(std::vector<GRAPHICS::Line> & lines)
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	unsigned int index = 0;


	for (auto it = lines.begin(); it != lines.end(); it++) {

		GRAPHICS::Line temp = *it;
		//start line
		vertices.push_back(temp.start.x);
		vertices.push_back(temp.start.y);
		vertices.push_back(temp.start.z);
		//end of line
		vertices.push_back(temp.finish.x);
		vertices.push_back(temp.finish.y);
		vertices.push_back(temp.finish.z);

		//index
		indices.push_back(index);
		indices.push_back(index + 1);
		index += 2;
	}

	gl::BindVertexArray(m_wireVaoHandle);

	gl::GenBuffers(2, m_wireVboHandle);

	gl::BindBuffer(gl::ARRAY_BUFFER, m_wireVboHandle[0]);
	gl::BufferData(gl::ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), (void*)&(vertices.at(0)), gl::STATIC_DRAW);

	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, NULL);


	gl::DrawElements(gl::LINES, indices.size(), gl::UNSIGNED_INT, (void*)&(indices.at(0)));

	gl::DisableVertexAttribArray(0);
	lines.clear();
	vertices.clear();
	indices.clear();
	PHYSICS::PhysicsController::GetPhysicsInstance().GetDebugDrawer()->ClearLines();
	
}

void GAME::Game::PollKeyEvents()
{
	UTIL::EventHandler::getInstance().PollKeyEvents(m_window.GetWindow());
}

void GAME::Game::CurrentCamera()
{
	m_cameraStates = static_cast<CameraStates>(UTIL::EventHandler::getInstance().GetCameraState());
	
	switch (m_cameraStates)
	{
	case STANDARD_VIEW:
		m_CameraController = m_standardCamera;
		UTIL::EventHandler::getInstance().setCamera(m_CameraController);
	
		break;
	case FIRST_PERSON_VIEW:
		m_CameraController = m_firstPersonCamera;
		UTIL::EventHandler::getInstance().setCamera(m_CameraController);
		break;
	case REAR_VIEW:
		m_CameraController = m_rearViewCamera;
		UTIL::EventHandler::getInstance().setCamera(m_CameraController);
	
		break;
	case FREE_ROAM:
		
		break;
	default:
		break;
	}
	
	
}

void GAME::Game::MemoryPoolTest()
{
	int loop = 100000;
	DWORD count = GetTickCount();
	std::vector<GRAPHICS::Particle*> p;
	for (int t = 0; t < 100; t++)
	{
		p.push_back(new GRAPHICS::Particle());
	}

	for (int testCount = 1; testCount <= 20; testCount++)
	{
		 int i;
		count = GetTickCount();

		for (i = 0; i < loop * testCount; i++)
		{
			//Using memory pool		
			m_particleMemoryManager.create(glm::vec3(1.0f), glm::vec3(1.0f), 1); 
			// life set to one, so we can instantly deallocate for the test.
			// memory pool updating
			m_particleMemoryManager.update();
		}

		UTIL::LOG(UTIL::LOG::INFO) << "Repeat " << loop * testCount << " Times " << "Using memory pool, the Interval = " << GetTickCount() - count << " ms";

		count = GetTickCount();

		for (i = 0; i < loop * testCount; i++)
		{
				
				p[0] = new GRAPHICS::Particle();	//No memory pool
				p[0]->Init(glm::vec3(1.0f), glm::vec3(1.0f), 1);

				for (int j = 0; j < 100; j++)
				{
					if (p.at(j)->Update())
					{
						if(!p.at(j)->Alive())
						delete p[j];
					}
				}
		}

		UTIL::LOG(UTIL::LOG::INFO) << "Repeat " << loop * testCount << " Times " << "NOT using the memory pool, the Interval = " << GetTickCount() - count << " ms";
		UTIL::LOG(UTIL::LOG::INFO) << "";
	}

}

void GAME::Game::MainMenu(bool loaded)
{
	m_menuShader->Use();

	gl::Clear(gl::COLOR_BUFFER_BIT);
	// Load and create a texture 
	
	
	gl::GenTextures(1, &m_menuTexture);
	gl::BindTexture(gl::TEXTURE_2D, m_menuTexture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
											// Set our texture parameters
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);	// Set texture wrapping to GL_REPEAT
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
	// Set texture filtering
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	// Load, create texture and generate mipmaps
	int m_loadingScreenW, m_loadingScreenH, m_menuScreenW, m_menuScreenH;
	unsigned char* m_loadingImage = SOIL_load_image("./external/assets/bg/HavocSpeed.png", &m_loadingScreenW, &m_loadingScreenH, 0, SOIL_LOAD_RGBA);;
	unsigned char* m_menuImage = SOIL_load_image("./external/assets/bg/HavocSpeedMenu.png", &m_menuScreenW, &m_menuScreenH, 0, SOIL_LOAD_RGBA);
	if (loaded == false)
	{
		gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, m_loadingScreenW, m_loadingScreenH, 0, gl::RGBA, gl::UNSIGNED_BYTE, m_loadingImage);
	}
	else if (loaded == true)
	{
		gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, m_menuScreenW, m_menuScreenH, 0, gl::RGBA, gl::UNSIGNED_BYTE, m_menuImage);
	}

	gl::GenerateMipmap(gl::TEXTURE_2D);
	m_menuShader->SetUniform("menu", m_menuTexture);
	gl::BindTexture(gl::TEXTURE_2D, 0);

	// Array for full-screen quad
	GLfloat verts[] = {
		
		-1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,	
	};
	GLfloat tc[] = {
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f,0.0f,
		1.0f, 1.0f,
	};

	// Set up the buffers
	unsigned int handle[2];
	gl::GenBuffers(2, handle);
	gl::BindBuffer(gl::ARRAY_BUFFER, handle[0]);
	gl::BufferData(gl::ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, gl::STATIC_DRAW);
	gl::BindBuffer(gl::ARRAY_BUFFER, handle[1]);
	gl::BufferData(gl::ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, gl::STATIC_DRAW);
	// Set up the vertex array object
	gl::GenVertexArrays(1, &m_quad);
	gl::BindVertexArray(m_quad);
	gl::BindBuffer(gl::ARRAY_BUFFER, handle[0]);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, ((GLubyte *)NULL + (0)));
	gl::EnableVertexAttribArray(0);  // Vertex position
	gl::BindBuffer(gl::ARRAY_BUFFER, handle[1]);
	gl::VertexAttribPointer(1, 2, gl::FLOAT, FALSE, 0, ((GLubyte *)NULL + (0)));
	gl::EnableVertexAttribArray(1);  // Texture coordinates
	gl::BindVertexArray(0);

}

void GAME::Game::RenderMainMenu()
{
	m_menuShader->Use();
	
	gl::Clear(gl::COLOR_BUFFER_BIT);

	gl::BindVertexArray(m_quad);
	gl::BindTexture(gl::TEXTURE_2D, m_menuTexture);
	gl::DrawArrays(gl::TRIANGLES, 0, 6);
	m_window.Display();
}
