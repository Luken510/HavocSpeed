#pragma once
#include <gl.hpp>
#include <GLFW/glfw3.h>
#include <memory>
#include "cameraBase.h"
#include "QuatCamera.h"
#include "raceCar.h"

#define NUM_OF_CAMERAS 4


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

		static void ResizeCallBack(GLFWwindow * window, int width, int height);

		void ResizeCallBackImp(GLFWwindow * window, int width, int height);


		void setCamera(std::shared_ptr<UTIL::CAMERA::CameraBase> Camera);
		void setCamera(std::shared_ptr<UTIL::CAMERA::QuatCamera> Camera);

		void setCar(std::shared_ptr<RaceCar> car);
		void setPlayer2Car(std::shared_ptr<RaceCar> car);

		void PollKeyEvents(GLFWwindow * window);

		bool GetWireBool();
		int GetCameraState();
		int GetGameState();
		void SetGameState(int i);
		float GetWindowWidth();
		float GetWindowHeight();

	private:
		EventHandler();
		 // private constructor necessary to allow only 1 instance
		EventHandler(EventHandler const&); // prevent copies
		void operator=(EventHandler const&); // prevent assignments

		std::shared_ptr<UTIL::CAMERA::CameraBase> m_camera;
		std::shared_ptr<UTIL::CAMERA::QuatCamera> m_quatCamera;
		std::shared_ptr<RaceCar> m_car;
		std::shared_ptr<RaceCar> m_car2;
		
		bool m_boolForWireMode;
		int m_cameraStateController;
		int m_gameStateController;

		float m_width;
		float m_height;
		
	};

}