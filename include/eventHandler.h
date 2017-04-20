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
		/*!
		\brief getInstance returns the instance to be used - Singelton.
		*/
		static EventHandler& getInstance();
		
		/*!
		\brief Static MouseButtonCallback used to get around GLFW3
		*/
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods); 
		/*!
		\brief MouseButtonCallbackImpl used to handle mouse input
		*/
		void MouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods); //this is the actual implementation of the callback method
		/*!
		\brief Static ScrollButtonCallBack used to get around GLFW3
		*/
		static void ScrollButtonCallBack(GLFWwindow *window, double x, double y);
		/*!
		\brief ScrollButtonCallBackImp used to handle mouse scroll input
		*/
		void ScrollButtonCallBackImp(GLFWwindow *window, double x, double y);
		/*!
		\brief Static KeyCallBack used to get around GLFW3
		*/
		static void KeyCallBack(GLFWwindow * window, int key, int cancode, int action, int mods);
		/*!
		\brief KeyCallBackImp used to handle keyboard input
		*/
		void KeyCallBackImp(GLFWwindow * window, int key, int cancode, int action, int mods);
		/*!
		\brief Static ResizeCallBack used to get around GLFW3
		*/
		static void ResizeCallBack(GLFWwindow * window, int width, int height);
		/*!
		\brief ResizeCallBackImp used to resize the screen
		*/
		void ResizeCallBackImp(GLFWwindow * window, int width, int height);

		/*!
		\brief setCamera used to setCamera to CameraBase
		*/
		void setCamera(std::shared_ptr<UTIL::CAMERA::CameraBase> Camera);
		/*!
		\brief setCamera used to setCamera to QuatCamera
		*/
		void setCamera(std::shared_ptr<UTIL::CAMERA::QuatCamera> Camera);

		/*!
		\brief setCar used to setCar
		*/
		void setCar(std::shared_ptr<RaceCar> car);
		/*!
		\brief setPlayer2Car used to setPlayer2Car
		*/
		void setPlayer2Car(std::shared_ptr<RaceCar> car);

		/*!
		\brief PollKeyEvents used to listen to event calls
		\param window the current rendered window.
		*/
		void PollKeyEvents(GLFWwindow * window);

		/*!
		\brief GetWireBool used to listen to retrieve whether or not the game should render wireframe mode
		*/
		bool GetWireBool();
		/*!
		\brief GetCameraState used to change the camera aka the view.
		*/
		int GetCameraState();
		/*!
		\brief GetGameState used to listen gamestate changes
		*/
		int GetGameState();
		/*!
		\brief SetGameState used to enact game state changes
		*/
		void SetGameState(int i);
		/*!
		\brief GetWindowWidth used to retrive the window width
		*/
		float GetWindowWidth();
		/*!
		\brief GetWindowHeight used to retrive the window height
		*/
		float GetWindowHeight();


	private:
		/*!
		\brief Constructor - singleton
		*/
		EventHandler();
		/*!
		\brief EventHandler overloaded to prevent copies of the instance
		*/
		EventHandler(EventHandler const&);
		/*!
		\brief Overloaded = - to prevent assignments
		*/
		void operator=(EventHandler const&); 

		std::shared_ptr<UTIL::CAMERA::CameraBase> m_camera; //!< Current camera to manipulate
		std::shared_ptr<UTIL::CAMERA::QuatCamera> m_quatCamera; //!< Current camera to manipulate
		std::shared_ptr<RaceCar> m_car;  //!< Current car to manipulate
		std::shared_ptr<RaceCar> m_car2; //!< Current car to manipulate
		
		bool m_boolForWireMode; //!< Whether to render in WireFrameMode or not.
		int m_cameraStateController;//!< Camera controller
		int m_gameStateController;//!< GameState controller

		float m_width; //!< width of the window
		float m_height; //!< height of the window
		
	};

}