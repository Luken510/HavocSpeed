#include <iostream>
#include <memory>

#include "eventHandler.h"
#include "QuatCamera.h"

UTIL::EventHandler & UTIL::EventHandler::getInstance()
{
	
		static EventHandler instance; // lazy singleton, instantiated on first use
		return instance;
	
}

void UTIL::EventHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	
		//here we access the instance via the singleton pattern and forward the callback to the instance method
		getInstance().mouseButtonCallbackImpl(window, button, action, mods);
	
}

void UTIL::EventHandler::mouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods)
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

void UTIL::EventHandler::setCamera(std::shared_ptr<UTIL::QuatCamera> Camera)
{
	
	camera = Camera;
	
}

UTIL::EventHandler::EventHandler()
{
}

void UTIL::EventHandler::operator=(EventHandler const &)
{
}