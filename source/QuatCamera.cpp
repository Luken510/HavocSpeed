
#include "QuatCamera.h"
#include <cmath>
#include <btBulletDynamicsCommon.h>

#include <iostream>






	UTIL::CAMERA::QuatCamera::QuatCamera() : m_follow(false), m_turning(false)
	{
		reset();
	}

	UTIL::CAMERA::QuatCamera::QuatCamera(const glm::vec3& position) : m_follow(false), m_turning(false)
	{
		//_projection = glm::mat4(1.0f);
		reset();
		m_position = position;
	}


	const glm::vec3& UTIL::CAMERA::QuatCamera::position() const
	{
		return m_position;
	}

	void UTIL::CAMERA::QuatCamera::setPosition(const glm::vec3& position)
	{
		m_position = position;
	}

	float UTIL::CAMERA::QuatCamera::fieldOfView() const
	{
		return m_fieldOfView;
	}


	void UTIL::CAMERA::QuatCamera::setFieldOfView(float fieldOfView)
	{
		assert(fieldOfView>0.0f && fieldOfView <180.0f);
		m_fieldOfView = fieldOfView;
	}


	float UTIL::CAMERA::QuatCamera::aspectRatio() const
	{
		return m_aspectRatio;
	}

	void UTIL::CAMERA::QuatCamera::SetAspectRatio(float aspectRatio)
	{
		assert(aspectRatio >0.0f);
		m_aspectRatio = aspectRatio;
	}


	float UTIL::CAMERA::QuatCamera::nearPlane() const
	{
		return m_nearPlane;
	}

	float UTIL::CAMERA::QuatCamera::farPlane() const
	{
		return m_farPlane;
	}

	

	void UTIL::CAMERA::QuatCamera::setNearAndFarPlanes(float nearPlane, float farPlane)
	{
		assert(nearPlane > 0.0f);
		assert(farPlane > nearPlane);
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;
	}

///////////////////////////////////////////////////////////////////////////////////////////
// Generate a quaternion from axis and rotation angle in radians
//This is to construct the rotation quaternion
////////////////////////////////////////////////////////////////////////////////////////////

	glm::quat fromAxisAngle(glm::vec3 axis, float angle)
	{
		glm::quat rotation;
		

		rotation.w = cos((angle / 2));
		rotation.x = sin((angle / 2)) * axis[0];
		rotation.y = sin((angle / 2)) * axis[1];
		rotation.z = sin((angle / 2)) * axis[2];

		return rotation;

	}

/////////////////////////////////////////////////////////////////////////////////////////////
//Rotate the camera
//Orient according to required pitch and yaw
/////////////////////////////////////////////////////////////////////////////////////////////
	void UTIL::CAMERA::QuatCamera::rotate(const float yaw, const float pitch)
	{

		glm::quat qyPitch = fromAxisAngle(WORLDX, pitch);
		glm::quat qxYaw = fromAxisAngle(WORLDY, yaw);

		glm::quat qOutput = qyPitch * m_orientation * qxYaw; //pitch is effected by the local variables (_orientation)
			// yaw we want only affected by world co-ords which means we dont want it effect by orient i.e is doesnt use them (opengl goes backwards remember.)
		
		glm::normalize(qOutput);

		m_orientation = qOutput;

		updateView();

	}

	
	void UTIL::CAMERA::QuatCamera::pan(const float x, const float y)
	{
		
		m_position += m_xaxis * x;
		m_position += m_yaxis * -y;
		
		
		updateView();
	}

	void UTIL::CAMERA::QuatCamera::zoom(const float z)
	{

		m_position += m_zaxis * z;
		
		
		updateView();

	}


	void UTIL::CAMERA::QuatCamera::updateView()
	{
		//Construct the view matrix from orientation quaternion and position vector

		//First get the matrix from the 'orientaation' Quaternion
		//This deals with the rotation and scale part of the view matrix
		m_view = glm::mat4_cast(m_orientation); // Rotation and Scale

		
		//Extract the camera coordinate axes from this matrix
		m_xaxis = glm::vec3(m_view[0][0], m_view[1][0], m_view[2][0]);
		m_yaxis = glm::vec3(m_view[0][1], m_view[1][1], m_view[2][1]);
		m_zaxis = glm::vec3(m_view[0][2], m_view[1][2], m_view[2][2]);

		//And use this and current camera position to set the translate part of the view matrix
		m_view[3][0] = -glm::dot(m_xaxis, m_position); //Translation x
		m_view[3][1] = -glm::dot(m_yaxis, m_position); //Translation y
		m_view[3][2] = -glm::dot(m_zaxis, m_position); //Translation z
		
	}

	void UTIL::CAMERA::QuatCamera::Update(double deltaTime, glm::mat4 target)
	{
		if (m_follow == true)
		{
			m_carPos.x = target[3][0];
			m_carPos.y = target[3][1];
			m_carPos.z = target[3][2];

			m_orientationTemp = glm::quat_cast(target);

			m_projection = glm::perspective(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);

			updateView();

		}
	}

	void UTIL::CAMERA::QuatCamera::Follow(double deltaTime, glm::vec3 carVelocity, glm::mat4 target, float angle)
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


	void UTIL::CAMERA::QuatCamera::roll(const float z)
	{

		glm::quat qzRoll = fromAxisAngle(WORLDZ, z);

		glm::quat zOutput = m_orientation * qzRoll;

		glm::normalize(zOutput);

		m_orientation = zOutput;

		updateView();

	}

	void UTIL::CAMERA::QuatCamera::reset(void)
	{
		//Initialise camera axes
		m_xaxis = WORLDX;
		m_yaxis = WORLDY;
		m_zaxis = WORLDZ;

		//Initialise camera position 
		m_position= glm::vec3(0.0f,2.0f,20.0f);

		//Initialise the orientation
		m_orientation = glm::quat(1.0,0.0,0.0,0.0);

		//Initialise camera perspective parameters
		m_fieldOfView = glm::radians(50.0f);
		m_nearPlane = 0.01f;
		m_farPlane = 1000.0f;
		m_aspectRatio = 4.0f/3.0f;

		m_projection = glm::perspective(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);

		updateView();

	}


	glm::mat4 UTIL::CAMERA::QuatCamera::View()
	{
	
		this->updateView();
		return m_view;
	}

	glm::mat4 UTIL::CAMERA::QuatCamera::Projection()
	{

		return m_projection;
		
	}

	
