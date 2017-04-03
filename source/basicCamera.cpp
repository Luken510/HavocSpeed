#include "basicCamera.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

UTIL::CAMERA::BasicCamera::BasicCamera(float windowHeight, float windowWidth)
	: m_view(glm::mat4(1.0f)), m_projection(glm::mat4(1.0f)), m_windowHeight(windowHeight), m_windowWidth(windowWidth),
		m_nearPlane(0.1f), m_farPlane(1000.0f), m_following(false)
{
	m_aspectRatio = m_windowHeight / m_windowWidth;
}

UTIL::CAMERA::BasicCamera::BasicCamera() 
	: m_view(glm::mat4(1.0f)), m_projection(glm::mat4(1.0f)), m_windowHeight(1280), m_windowWidth(720),
		m_nearPlane(0.1f), m_farPlane(1000.0f), m_following(false)
{
	m_aspectRatio = m_windowHeight / m_windowWidth;
}

UTIL::CAMERA::BasicCamera::~BasicCamera()
{
}

void UTIL::CAMERA::BasicCamera::Update(double deltaTime)
{

}

void UTIL::CAMERA::BasicCamera::Follow(double deltaTime, glm::vec3 carVelocity, glm::mat4 target)
{
	/*glm::mat3 targetRotation(target);
	glm::vec3 targetPosition(target[3]); // target is a 4x4 matrix holding both its current rotation and position, position being held in [3][0] [3][1] [3][2]
	
	m_view = glm::lookAt(glm::vec3(targetPosition.x, targetPosition.y - 3, targetPosition.z - 5), targetPosition, glm::vec3(0.0f, 1.0f, 0.0f));
	*/
	m_projection = glm::perspective(90.0f, m_aspectRatio, m_nearPlane, m_farPlane);

	const glm::vec3 lookAtPos(0, 1, 0);
	const glm::vec3 lookFromPos(0, 1, 1);  // 0, 3, -2


	float viewDistance = 20.5f; // 10.f

	glm::mat3 targetRot(target);
	glm::vec3 targetPos(target[3]);

	// Using statics here because of lazy programming
	static glm::vec3 lastBaseLookFromPos = targetPos - targetRot[2] * 20.f;
	static float facing = 1.f;
	//static float currentSettleTime = 0.f;

	// First we nudge the previous camera position to get it to behave exactly the way we want.
	// If the object is moving towards the camera then nudge the camera
	// side was a little so it goes around.
	// This avoids camera going over the top of the car and flipping when 
	// changing velocity when going into reverse while looking forwards.
	//float nudge = glm::length(lastBaseLookFromPos - targetPos);
//	if (nudge < viewDistance)
//	{
	//	nudge = viewDistance - nudge;
	//	lastBaseLookFromPos -= targetRot[0] * nudge * 1.1f;
	//	lastBaseLookFromPos.y += nudge * 0.2f;
//	}

	// Nudged the camera a bit so that it swings around a bit more to face
	// the direction that the car is moving.
	// This makes it a little easier to see around corners.
	lastBaseLookFromPos -= carVelocity * (float)(deltaTime);

	// Nudge the camera so that is a bit more likely to look in the direction
	// that the car is facing. This helps most when maneuvering at low speed.
	glm::vec3 facingNudge = targetRot[2] * 0.2f * facing;
	facingNudge.y = 0.f;
	lastBaseLookFromPos -= facingNudge;

	// Smoothly choose whether the car is facing forwards or backwards for the above code.
	float dot = glm::dot(targetPos - lastBaseLookFromPos, targetRot[2]);
	facing = dot * 0.1f;
	if (facing >  1.f) facing = 1.f;  // ClampPosNeg
	else if (facing < -1.f) facing = -1.f;

	// Now we make the camera follow the new object position.
	glm::vec3 lookDirection = targetPos - lastBaseLookFromPos;
	float mag = glm::length(lookDirection);
	if (mag > 0.f)
		lookDirection /= mag;
	else
		lookDirection = glm::vec3(0, 0, 1);   // It would be much better to use the last valid value

	glm::vec3 currentLookFromPos = targetPos - lookDirection * viewDistance;

	lastBaseLookFromPos = currentLookFromPos;

	if (glm::abs(lookDirection.y) <= 0.99f)
	{
		glm::mat4 tmp = glm::lookAt(glm::vec3(0), lookDirection, glm::vec3(0, 1, 0));
		glm::mat3 m(tmp);

		currentLookFromPos += lookFromPos * m;
		glm::vec3 currentLookAtPos = lookDirection;
		// glm::vec3 currentLookAtPos = targetPos + lookAtPos * m;

		m_view = glm::lookAt(currentLookFromPos, currentLookAtPos, glm::vec3(0, 1, 0));
	}

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

	m_projection = glm::perspective(50.0f, m_aspectRatio, m_nearPlane, m_farPlane);
}
