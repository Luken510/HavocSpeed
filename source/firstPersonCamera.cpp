#include "firstPersonCamera.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <cmath>

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
	////http://nghiaho.com/?page_id=846
	//double angle = std::atan2(target[3][1], std::sqrt((target[3][2] * target[3][2]) + (target[3][3] * target[3][3])));
	//glm::mat4 CameraPosition = glm::translate(glm::vec3(targetPosition.x, targetPosition.y, targetPosition.z)) * glm::rotate((float)angle, glm::vec3(0.0f, 1.0f, 0.0f));

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
//	m_view[3][2] += 5;
	//m_view = glm::lookAt(glm::mat3(CameraPosition), targetPosition, glm::vec3(0.0f, 1.0f, 0.0f));

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

	m_projection = glm::perspective(90.0f, m_aspectRatio, m_nearPlane, m_farPlane);
}
