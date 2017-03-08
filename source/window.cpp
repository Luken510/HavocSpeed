#include "window.h"

#include <glm/glm.hpp>

#include "logger.h"


GAME::Window::Window()
{
	
	//Select OpenGL4.3 with a forward compatible core profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, true);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_SAMPLES, 8);

	//Init GLFW
	if (!glfwInit())
	{
		UTIL::LOG(UTIL::LOG::FAULT) << "GLFW Failed to start";
		exit(EXIT_FAILURE);
	}

	//Open the window
	m_window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Havoc Speed", NULL, NULL); // try to make full screen

	if (!m_window) {
		glfwTerminate();
		UTIL::LOG(UTIL::LOG::FAULT) << " GLFW window failed to create";
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_window);

	gl::exts::LoadTest LoadFunctions = gl::sys::LoadFunctions();

	if (!LoadFunctions) {
		glfwTerminate();
		UTIL::LOG(UTIL::LOG::FAULT) << "OpenGL Functions failed to load";
		exit(EXIT_FAILURE);
	}

	UTIL::LOG(UTIL::LOG::DEBUG) << "Amount of OpenGL functions that haven't loaded : " << LoadFunctions.GetNumMissing();

	gl::Viewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

	gl::Enable(gl::MULTISAMPLE);
	gl::ClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	
}

GAME::Window::~Window()
{
	glfwTerminate();
}

void GAME::Window::Display()
{
	glfwSwapBuffers(m_window);
}

GLFWwindow* GAME::Window::GetWindow()
{
	return m_window;
}


