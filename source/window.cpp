#include "window.h"

#include <glm/glm.hpp>

#include "logger.h"


GAME::Window::Window() : lastCursorPositionX(0), lastCursorPositionY(0), cursorPositionX(0), cursorPositionY(0)
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

void GAME::Window::resizeGL(std::shared_ptr<UTIL::QuatCamera> camera, int w, int h)
{
	gl::Viewport(0, 0, w, h);
	m_height = h;
	m_width = w;
	camera->setAspectRatio((float)w / h);
}

void GAME::Window::update(float deltaTime, std::shared_ptr<UTIL::QuatCamera> camera)
{
	//Get the current cursor position
	glfwGetCursorPos(m_window, &cursorPositionX, &cursorPositionY);
	//See how much the cursor has moved
	float deltaX = (float)(lastCursorPositionX - cursorPositionX);
	float deltaY = (float)(lastCursorPositionY - cursorPositionY);

	//Using a different way (i.e. instead of callback) to check for LEFT mouse button
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT))
	{
		//Rotate the camera. The 0.001f is a velocity mofifier to make the speed sensible

		camera->rotate(deltaX*ROTATE_VELOCITY, deltaY*ROTATE_VELOCITY);

	}

	//Using a different way (i.e. instead of callback) to check for RIGHT mouse button
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT))
	{

		//Rotate the camera. The 0.01f is a velocity mofifier to make the speed sensible
		camera->pan(deltaX*MOVE_VELOCITY, deltaY*MOVE_VELOCITY);

	}
	//To adjust Roll with MIDDLE mouse button
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_MIDDLE))
	{

		camera->roll(deltaX*ROTATE_VELOCITY);
	}

	//Store the current cursor position
	lastCursorPositionX = cursorPositionX;
	lastCursorPositionY = cursorPositionY;
}

