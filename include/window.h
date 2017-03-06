#pragma once

#include <memory>
#include <gl.hpp>
#include <GLFW/glfw3.h>

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

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
	private:
		GLFWwindow* m_window;//!< Pointer to the GLFW window.
		int			m_width;//!< Width of the window
		int			m_height;//!< Heightof the window
	};
}