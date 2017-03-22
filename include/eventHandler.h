#pragma once
#include <gl.hpp>
#include <GLFW/glfw3.h>
#include <memory>
#include "raceCar.h"
#include "QuatCamera.h"


// worth mentioning that this implementation won't work on multiple glfw windows
namespace UTIL {
	class EventHandler
	{
	public:
		static EventHandler& getInstance(); // Singleton is accessed via getInstance()
		

		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods); // this method is specified as glfw callback
	

		void MouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods); //this is the actual implementation of the callback method
		
		static void ScrollButtonCallBack(GLFWwindow *window, double x, double y);
		
		void ScrollButtonCallBackImp(GLFWwindow *window, double x, double y);
		
		static void KeyCallBack(GLFWwindow * window, int key, int cancode, int action, int mods);

		void KeyCallBackImp(GLFWwindow * window, int key, int cancode, int action, int mods);

		void setCamera(std::shared_ptr<UTIL::QuatCamera> Camera);
		void setCar(std::shared_ptr<RaceCar> car);

		bool getWireBool();
		
	private:
		EventHandler();
		 // private constructor necessary to allow only 1 instance
		EventHandler(EventHandler const&); // prevent copies
		void operator=(EventHandler const&); // prevent assignments

		std::shared_ptr<UTIL::QuatCamera> camera;
		std::shared_ptr<RaceCar> m_car;
		
		bool m_boolForWireMode;
		
	};

}