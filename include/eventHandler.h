#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include "QuatCamera.h"
namespace UTIL {
	class EventHandler
	{
	public:
		static EventHandler& getInstance(); // Singleton is accessed via getInstance()
		

		static void mouseButtonCallback(int key, int action); // this method is specified as glfw callback
	

		void mouseButtonCallbackImpl(int key, int action); //this is the actual implementation of the callback method
		
		static void ScrollButtonCallBack(GLFWwindow *window, double x, double y);
		
		void ScrollButtonCallBackImp(GLFWwindow *window, double x, double y);
		
		void setCamera(std::shared_ptr<UTIL::QuatCamera> Camera);
		
	private:
		EventHandler();
		 // private constructor necessary to allow only 1 instance
		EventHandler(EventHandler const&); // prevent copies
		void operator=(EventHandler const&); // prevent assignments

		std::shared_ptr<UTIL::QuatCamera> camera;
		
		
	};

}