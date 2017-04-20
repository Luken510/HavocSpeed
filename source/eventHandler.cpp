#include <iostream>
#include <memory>

#include "eventHandler.h"
#include "QuatCamera.h"

UTIL::EventHandler & UTIL::EventHandler::getInstance()
{
	
		static EventHandler instance; //singleton, instantiated on first use
		return instance;
	
}

void UTIL::EventHandler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	
		//here we access the instance via the singleton pattern and forward the callback to the instance method
		getInstance().MouseButtonCallbackImpl(window, button, action, mods);

		
	
}

void UTIL::EventHandler::MouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods)
{
	
		//the callback is handled in this instance method           
		//... [CODE here]

	/*if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double cursorPosX, cursorPosY;
		glfwGetCursorPos(window, &cursorPosX, &cursorPosY);

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		cursorPosX = (2.0f * cursorPosX) / width - 1.0f;
		cursorPosY = 1.0f - (2.0f * cursorPosY) / height;

		glm::mat4 proj = camera->projection();
		glm::mat4 view = camera->view();

		glm::mat4 invVP = glm::inverse(proj * view);
		glm::vec4 screenPos = glm::vec4(cursorPosX, -cursorPosY, 1.0f, 1.0f);
		glm::vec4 worldPos = invVP * screenPos;

		glm::vec3 dir = glm::normalize(glm::vec3(worldPos));
	}*/

	// attempt at getting window co-ords, will be better if you implement a raycast to get position of objects/world etc;
	//http://stackoverflow.com/questions/29997209/opengl-c-mouse-ray-picking-glmunproject
	//http://antongerdelan.net/opengl/raycasting.html

	double cursorPositionX, cursorPositionY;

	glfwGetCursorPos(window, &cursorPositionX, &cursorPositionY);
	if (getInstance().GetGameState() == 0)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			if (cursorPositionX >= 265 && cursorPositionX <= 600 && cursorPositionY >= 185 && cursorPositionY <= 350)
			{
				UTIL::LOG(UTIL::LOG::INFO) << " 1 Player Mode Enabled";
				UTIL::LOG(UTIL::LOG::INFO) << " ";
				UTIL::LOG(UTIL::LOG::INFO) << " Loading Please Wait..";
				getInstance().SetGameState(1); // Normal 1 player mode
			}
			else if (cursorPositionX >= 820 && cursorPositionX <= 1160 && cursorPositionY >= 185 && cursorPositionY <= 350)
			{
				UTIL::LOG(UTIL::LOG::INFO) << " 2 Player Mode Enabled";
				UTIL::LOG(UTIL::LOG::INFO) << " ";
				UTIL::LOG(UTIL::LOG::INFO) << " Loading Please Wait..";
				m_car2->Init();
				getInstance().SetGameState(4); // Normal 2 player mode
			}
			else if (cursorPositionX >= 850 && cursorPositionX <= 1150 && cursorPositionY >= 390 && cursorPositionY <= 570)
			{
				UTIL::LOG(UTIL::LOG::INFO) << " Without Pattern Test Enabled";
				UTIL::LOG(UTIL::LOG::INFO) << " ";
				UTIL::LOG(UTIL::LOG::INFO) << " Loading Please Wait..";
				getInstance().SetGameState(3); // Without patterns Test
			}
			else if (cursorPositionX >= 321 && cursorPositionX <= 570 && cursorPositionY >= 390 && cursorPositionY <= 560)
			{
				UTIL::LOG(UTIL::LOG::INFO) << " Pattern Test Enabled";
				UTIL::LOG(UTIL::LOG::INFO) << " ";
				UTIL::LOG(UTIL::LOG::INFO) << " Loading Please Wait..";
				getInstance().SetGameState(2); // With Patterns Test
			}
		}
	}
	
	
}

void UTIL::EventHandler::ScrollButtonCallBack(GLFWwindow * window, double x, double y)
{
	
	getInstance().ScrollButtonCallBackImp(window, x, y);
	
	
}

void UTIL::EventHandler::ScrollButtonCallBackImp(GLFWwindow * window, double x, double y)
{
	if (getInstance().GetGameState() != 0)
	{
		if (m_cameraStateController == 3)
			m_quatCamera->zoom((float)y*1.5f);
	}
	
}

void UTIL::EventHandler::KeyCallBack(GLFWwindow * window, int key, int cancode, int action, int mods)
{
	getInstance().KeyCallBackImp(window, key, cancode, action, mods);
}

void UTIL::EventHandler::KeyCallBackImp(GLFWwindow * window, int key, int cancode, int action, int mods)
{
	if (getInstance().GetGameState() != 0)
	{
		if (key == 'V' && action == GLFW_RELEASE)
		{

			if (m_boolForWireMode == false)
				m_boolForWireMode = true;
			else
				m_boolForWireMode = false;
			UTIL::LOG(UTIL::LOG::INFO) << " Key Pressed = 'V'";
		}

		if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
		{
			m_car->Fire();
		}

		if (key == GLFW_KEY_TAB && action == GLFW_RELEASE)
		{

			if (m_cameraStateController < 0 && m_cameraStateController >= NUM_OF_CAMERAS)
			{
				UTIL::LOG(UTIL::LOG::FAULT) << " Camerastate out of scope, resetting to default.'";
				m_cameraStateController = 0;
			}

			if (m_cameraStateController == NUM_OF_CAMERAS - 1)
				m_cameraStateController = 0;
			else
				m_cameraStateController += 1;
			UTIL::LOG(UTIL::LOG::INFO) << " Key Pressed = 'TAB'";

			switch (m_cameraStateController)
			{
			case(0):
				UTIL::LOG(UTIL::LOG::INFO) << " Camera = Standard Camera";
				break;
			case(1):
				UTIL::LOG(UTIL::LOG::INFO) << " Camera = First Person Camera";
				break;
			case(2):
				UTIL::LOG(UTIL::LOG::INFO) << " Camera = Rear View Camera";
				break;
			case(3):
				UTIL::LOG(UTIL::LOG::INFO) << " Camera = Free Roam Camera";
				break;
			default:
				break;
			}
		}
	}

}

void UTIL::EventHandler::ResizeCallBack(GLFWwindow * window, int width, int height)
{
	getInstance().ResizeCallBackImp(window, width, height);
}

void UTIL::EventHandler::ResizeCallBackImp(GLFWwindow * window, int width, int height)
{
	gl::Viewport(0, 0, width, height);
	m_width = (float)width;
	m_height = (float)height;
	m_camera->SetAspectRatio((float)width / height);
	m_quatCamera->SetAspectRatio((float)width / height);

}

void UTIL::EventHandler::setCamera(std::shared_ptr<UTIL::CAMERA::CameraBase> Camera)
{
	m_camera = Camera;
}

void UTIL::EventHandler::setCamera(std::shared_ptr<UTIL::CAMERA::QuatCamera> Camera)
{

	m_quatCamera = Camera;

}

void UTIL::EventHandler::setCar(std::shared_ptr<RaceCar> car)
{
	m_car = car;
}

void UTIL::EventHandler::setPlayer2Car(std::shared_ptr<RaceCar> car)
{
	m_car2 = car;
}

void UTIL::EventHandler::PollKeyEvents(GLFWwindow * window)
{
	if (getInstance().GetGameState() != 0)
	{
		if (glfwGetKey(window, 'W'))
		{
			m_car->Drive();

		}
		if (glfwGetKey(window, 'S'))
		{
			m_car->Reverse();

		}
		if (glfwGetKey(window, 'A'))
		{
			m_car->TurnLeft();

		}
		if (glfwGetKey(window, 'D'))
		{
			m_car->TurnRight();
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE))
		{
			m_car->Brake();
		}

		if (getInstance().GetGameState() == 4)
		{
			//player 2 keys
			if (glfwGetKey(window, GLFW_KEY_UP))
			{
				m_car2->Drive();

			}
			if (glfwGetKey(window, GLFW_KEY_DOWN))
			{
				m_car2->Reverse();

			}
			if (glfwGetKey(window, GLFW_KEY_LEFT))
			{
				m_car2->TurnLeft();

			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT))
			{
				m_car2->TurnRight();
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL))
			{
				m_car2->Brake();
			}
			if (glfwGetKey(window, GLFW_KEY_0))
			{
				m_car2->Fire();
			}
		}
	}
}

bool UTIL::EventHandler::GetWireBool()
{
	return m_boolForWireMode;
}

int UTIL::EventHandler::GetCameraState()           
{
	return m_cameraStateController;
}

int UTIL::EventHandler::GetGameState()
{
	return m_gameStateController;
}

void UTIL::EventHandler::SetGameState(int i)
{
	m_gameStateController = i;
}

float UTIL::EventHandler::GetWindowWidth()
{
	return m_width;
}

float UTIL::EventHandler::GetWindowHeight()
{
	return m_height;
}

UTIL::EventHandler::EventHandler() : m_boolForWireMode(false), m_cameraStateController(0), m_gameStateController(5), m_width(1280), m_height(720)
{

}

void UTIL::EventHandler::operator=(EventHandler const &)
{
}
