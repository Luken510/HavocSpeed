#include "basicCamera.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

UTIL::CAMERA::BasicCamera::BasicCamera(float windowHeight, float windowWidth)
	: m_view(glm::mat4(1.0f)), m_projection(glm::mat4(1.0f)), m_windowHeight(windowHeight), m_windowWidth(windowWidth),
		m_nearPlane(0.1f), m_farPlane(1000.0f), m_following(false)
{
	m_aspectRatio = m_windowHeight / m_windowWidth;
	m_projection = glm::perspective(90.0f, m_aspectRatio, m_nearPlane, m_farPlane);
}

UTIL::CAMERA::BasicCamera::BasicCamera() 
	: m_view(glm::mat4(1.0f)), m_projection(glm::mat4(1.0f)), m_windowHeight(1280.0f), m_windowWidth(720.0f),
		m_nearPlane(0.1f), m_farPlane(1000.0f), m_following(false)
{
	m_aspectRatio = m_windowHeight / m_windowWidth;
	m_projection = glm::perspective(90.0f, m_aspectRatio, m_nearPlane, m_farPlane);
}

UTIL::CAMERA::BasicCamera::~BasicCamera()
{
}

void UTIL::CAMERA::BasicCamera::Update(double deltaTime)
{

}

void UTIL::CAMERA::BasicCamera::Follow(double deltaTime, glm::vec3 carVelocity, glm::mat4 target)
{
	glm::mat3 targetRotation(target);
	glm::vec3 targetPosition(target[3]); // target is a 4x4 matrix holding both its current rotation and position, position being held in [3][0] [3][1] [3][2]

	m_view = glm::mat4(targetRotation[0][0], targetRotation[1][0], targetRotation[2][0], 0,
		targetRotation[0][1], targetRotation[1][1], targetRotation[2][1], 0,
		targetRotation[0][2], targetRotation[1][2], targetRotation[2][2], 0,
		0, 0, 0, 1);

	//Extract the camera coordinate axes from this matrix
	glm::vec3 xaxis = glm::vec3(m_view[0][0], m_view[1][0], m_view[2][0]);
	glm::vec3 yaxis = glm::vec3(m_view[0][1], m_view[1][1], m_view[2][1]);
	glm::vec3 zaxis = glm::vec3(m_view[0][2], m_view[1][2], m_view[2][2]);

	//And use this and current camera position to set the translate part of the view matrix
	m_view[3][0] = -glm::dot(xaxis, targetPosition); //Translation x
	m_view[3][1] = -glm::dot(yaxis, targetPosition); //Translation y
	m_view[3][2] = -glm::dot(zaxis, targetPosition); //Translation z

	m_view[3][1] -= 2;
	m_view[3][2] -= 2;
}


glm::mat4 UTIL::CAMERA::BasicCamera::GetView()
{
	return m_view;
}

glm::mat4 UTIL::CAMERA::BasicCamera::GetProjection()
{
	return m_projection;
}

void UTIL::CAMERA::BasicCamera::SetAspectRatio(float aspectRatio)
{
	assert(aspectRatio > 0.0f);
	m_aspectRatio = aspectRatio;

	m_projection = glm::perspective(90.0f, m_aspectRatio, m_nearPlane, m_farPlane);
}
