#include "rearCamera.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

UTIL::CAMERA::RearCamera::RearCamera(float windowHeight, float windowWidth)
	: m_view(glm::mat4(1.0f)), m_projection(glm::mat4(1.0f)), m_windowHeight(windowHeight), m_windowWidth(windowWidth),
		m_nearPlane(0.1f), m_farPlane(1000.0f), m_following(false)
{
	m_aspectRatio = m_windowHeight / m_windowWidth;
}

UTIL::CAMERA::RearCamera::RearCamera()
{
}

UTIL::CAMERA::RearCamera::~RearCamera()
{
}

void UTIL::CAMERA::RearCamera::Update(double deltaTime)
{

}

void UTIL::CAMERA::RearCamera::Follow(double deltaTime, glm::vec3 carVelocity, glm::mat4 target)
{
	glm::mat3 targetRotation(target);
	glm::vec3 targetPosition(target[3]); // target is a 4x4 matrix holding both its current rotation and position, position being held in [3][0] [3][1] [3][2]
	
	m_view = glm::lookAt(glm::vec3(targetPosition.x + 7.0f, targetPosition.y + 10, targetPosition.z), targetPosition, glm::vec3(0.0f, 1.0f, 0.0f));

	m_projection = glm::perspective(90.0f, m_aspectRatio, m_nearPlane, m_farPlane);

}


glm::mat4 UTIL::CAMERA::RearCamera::GetView()
{
	return m_view;
}

glm::mat4 UTIL::CAMERA::RearCamera::GetProjection()
{
	return m_projection;
}

void UTIL::CAMERA::RearCamera::SetAspectRatio(float aspectRatio)
{
	assert(aspectRatio > 0.0f);
	m_aspectRatio = aspectRatio;

	m_projection = glm::perspective(90.0f, m_aspectRatio, m_nearPlane, m_farPlane);
}
