#pragma once

#include <memory>
#include <gl.hpp>
#include <GLFW/glfw3.h>

#include "QuatCamera.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define MOVE_VELOCITY 0.005f
#define ROTATE_VELOCITY 0.001f

namespace GAME {

	class Window {
	public:
		/*!
			\brief Constructor	
		*/
		Window();
		/*!
		\brief Destructor
		*/
		~Window();

		/*!
		\brief Display the current rendered screen
		*/
		void Display();

		/*!
		\brief get a pointer to the window
		*/
		GLFWwindow* GetWindow();
		
		void resizeGL(std::shared_ptr<UTIL::QuatCamera> camera, int w, int h);

		void update(float deltaTime, std::shared_ptr<UTIL::QuatCamera> camera);

		//To keep track of cursor location
		double lastCursorPositionX, lastCursorPositionY, cursorPositionX, cursorPositionY;

	private:
		GLFWwindow* m_window = nullptr;//!< Pointer to the GLFW window.
		Window* m_myWindow = nullptr;
		int			m_width;//!< Width of the window
		int			m_height;//!< Heightof the window
	};
}