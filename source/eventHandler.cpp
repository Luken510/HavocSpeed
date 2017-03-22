#include <iostream>
#include <memory>

#include "eventHandler.h"
#include "QuatCamera.h"
#include "raceCar.h"

UTIL::EventHandler & UTIL::EventHandler::getInstance()
{
	
		static EventHandler instance; // lazy singleton, instantiated on first use
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
	
}

void UTIL::EventHandler::ScrollButtonCallBack(GLFWwindow * window, double x, double y)
{
	
	getInstance().ScrollButtonCallBackImp(window, x, y);
	
	
}

void UTIL::EventHandler::ScrollButtonCallBackImp(GLFWwindow * window, double x, double y)
{
	
	camera->zoom((float)y*0.5f);
	
}

void UTIL::EventHandler::KeyCallBack(GLFWwindow * window, int key, int cancode, int action, int mods)
{
	getInstance().KeyCallBackImp(window, key, cancode, action, mods);
}

void UTIL::EventHandler::KeyCallBackImp(GLFWwindow * window, int key, int cancode, int action, int mods)
{
	if (key == 'W' &&  action == GLFW_PRESS)
	{
		m_car->Drive();
		UTIL::LOG(UTIL::LOG::INFO) << "Key Pressed = 'W'";
	}
	if (key == 'S' && action == GLFW_PRESS)
	{
		m_car->Reverse();
		UTIL::LOG(UTIL::LOG::INFO) << "Key Pressed = 'S'";
	}
	if (key == 'A' && action == GLFW_PRESS)
	{
		m_car->TurnLeft();
		UTIL::LOG(UTIL::LOG::INFO) << "Key Pressed = 'A'";
	}
	if (key == 'D' && action == GLFW_PRESS)
	{
		m_car->TurnRight();
		UTIL::LOG(UTIL::LOG::INFO) << "Key Pressed = 'D'";
	}

	// test which works better, press or release, add brake, make it work to test car physics.

	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
	{
		m_car->Drive();
		UTIL::LOG(UTIL::LOG::INFO) << " Key Pressed = 'UP'" ;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
	{
		m_car->Reverse();
		UTIL::LOG(UTIL::LOG::INFO) << " Key Pressed = 'DOWN'";
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
	{
		m_car->TurnLeft();
		UTIL::LOG(UTIL::LOG::INFO) << " Key Pressed = 'LEFT'";
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
	{
		m_car->TurnRight();
		UTIL::LOG(UTIL::LOG::INFO) << " Key Pressed = 'RIGHT'";
	}
	if (key == 'V' && action == GLFW_RELEASE)
	{
		
		if (m_boolForWireMode == false)
			m_boolForWireMode = true;
		else
			m_boolForWireMode = false;
		UTIL::LOG(UTIL::LOG::INFO) << " Key Pressed = 'V'";;
	}
}

void UTIL::EventHandler::setCamera(std::shared_ptr<UTIL::QuatCamera> Camera)
{
	
	camera = Camera;
	
}

void UTIL::EventHandler::setCar(std::shared_ptr<RaceCar> car)
{
	m_car = car;
}

bool UTIL::EventHandler::getWireBool()
{
	return m_boolForWireMode;
}

UTIL::EventHandler::EventHandler() : m_boolForWireMode(false)
{

}

void UTIL::EventHandler::operator=(EventHandler const &)
{
}
