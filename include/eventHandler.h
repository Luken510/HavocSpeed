#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include "QuatCamera.h"
namespace UTIL {
	// Input.h (the actual callback class for glfwSetMouseButtonCallback)
	class EventHandler
	{
	public:
		static EventHandler& getInstance() // Singleton is accessed via getInstance()
		{
			static EventHandler instance; // lazy singleton, instantiated on first use
			return instance;
		}

		static void mouseButtonCallback(int key, int action) // this method is specified as glfw callback
		{
			//here we access the instance via the singleton pattern and forward the callback to the instance method
			getInstance().mouseButtonCallbackImpl(key, action);
		}

		void mouseButtonCallbackImpl(int key, int action) //this is the actual implementation of the callback method
		{
			//the callback is handled in this instance method           
			//... [CODE here]
		}
		static void ScrollButtonCallBack(GLFWwindow *window, double x, double y)
		{
			getInstance().ScrollButtonCallBackImp(window, x, y);
		}
		void ScrollButtonCallBackImp(GLFWwindow *window, double x, double y)
		{
			camera->zoom((float)y*0.05);
		}

		void setCamera(std::shared_ptr<UTIL::QuatCamera> camera)
		{
			camera = camera;
		}
	private:
		EventHandler()
		{
		} // private constructor necessary to allow only 1 instance
		EventHandler(EventHandler const&); // prevent copies
		void operator=(EventHandler const&); // prevent assignments

		std::shared_ptr<UTIL::QuatCamera> camera;

		
	};

}