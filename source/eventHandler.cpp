#include <iostream>
#include <memory>

#include "eventHandler.h"
#include "QuatCamera.h"

UTIL::EventHandler & UTIL::EventHandler::getInstance()
{
	
		static EventHandler instance; // lazy singleton, instantiated on first use
		return instance;
	
}

void UTIL::EventHandler::mouseButtonCallback(int key, int action)
{
	
		//here we access the instance via the singleton pattern and forward the callback to the instance method
		getInstance().mouseButtonCallbackImpl(key, action);
	
}

void UTIL::EventHandler::mouseButtonCallbackImpl(int key, int action)
{
	
		//the callback is handled in this instance method           
		//... [CODE here]
	
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
