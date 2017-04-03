#include "firstPersonCamera.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

UTIL::CAMERA::firstPersonCamera::firstPersonCamera(float windowHeight, float windowWidth)
	: m_view(glm::mat4(1.0f)), m_windowHeight(windowHeight), m_windowWidth(windowWidth),
		m_nearPlane(0.1f), m_farPlane(1000.0f), m_following(false)
{

	m_aspectRatio = m_windowHeight / m_windowWidth;
	m_projection = glm::perspective(90.0f, m_aspectRatio, m_nearPlane, m_farPlane);
}

UTIL::CAMERA::firstPersonCamera::firstPersonCamera()
{
}

UTIL::CAMERA::firstPersonCamera::~firstPersonCamera()
{
}

void UTIL::CAMERA::firstPersonCamera::Update(double deltaTime)
{

}

void UTIL::CAMERA::firstPersonCamera::Follow(double deltaTime, glm::vec3 carVelocity, glm::mat4 target)
{
	glm::mat3 targetRotation(target);
	glm::vec3 targetPosition(target[3]); // target is a 4x4 matrix holding both its current rotation and position, position being held in [3][0] [3][1] [3][2]
	
	m_view = glm::lookAt(glm::vec3(targetPosition.x, targetPosition.y - 3, targetPosition.z - 5), targetPosition, glm::vec3(0.0f, 1.0f, 0.0f));

	m_projection = glm::perspective(90.0f, m_aspectRatio, m_nearPlane, m_farPlane);

}


glm::mat4 UTIL::CAMERA::firstPersonCamera::GetView()
{
	return m_view;
}

glm::mat4 UTIL::CAMERA::firstPersonCamera::GetProjection()
{
	return m_projection;
}

void UTIL::CAMERA::firstPersonCamera::SetAspectRatio(float aspectRatio)
{
	assert(aspectRatio > 0.0f);
	m_aspectRatio = aspectRatio;

	m_projection = glm::perspective(50.0f, m_aspectRatio, m_nearPlane, m_farPlane);
}
