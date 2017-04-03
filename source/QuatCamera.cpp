
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
		_position = position;
	}


	const glm::vec3& UTIL::CAMERA::QuatCamera::position() const
	{
		return _position;
	}

	void UTIL::CAMERA::QuatCamera::setPosition(const glm::vec3& position)
	{
		_position = position;
	}

	float UTIL::CAMERA::QuatCamera::fieldOfView() const
	{
		return _fieldOfView;
	}


	void UTIL::CAMERA::QuatCamera::setFieldOfView(float fieldOfView)
	{
		assert(fieldOfView>0.0f && fieldOfView <180.0f);
		_fieldOfView = fieldOfView;
	}


	float UTIL::CAMERA::QuatCamera::aspectRatio() const
	{
		return _aspectRatio;
	}

	void UTIL::CAMERA::QuatCamera::SetAspectRatio(float aspectRatio)
	{
		assert(aspectRatio >0.0f);
		_aspectRatio = aspectRatio;
	}


	float UTIL::CAMERA::QuatCamera::nearPlane() const
	{
		return _nearPlane;
	}

	float UTIL::CAMERA::QuatCamera::farPlane() const
	{
		return _farPlane;
	}

	

	void UTIL::CAMERA::QuatCamera::setNearAndFarPlanes(float nearPlane, float farPlane)
	{
		assert(nearPlane > 0.0f);
		assert(farPlane > nearPlane);
		_nearPlane = nearPlane;
		_farPlane = farPlane;
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

		glm::quat qOutput = qyPitch * _orientation * qxYaw; //pitch is effected by the local variables (_orientation)
			// yaw we want only affected by world co-ords which means we dont want it effect by orient i.e is doesnt use them (opengl goes backwards remember.)
		
		glm::normalize(qOutput);

		_orientation = qOutput;

		updateView();

	}

	
	void UTIL::CAMERA::QuatCamera::pan(const float x, const float y)
	{
		
		_position += _xaxis * x;
		_position += _yaxis * -y;
		
		
		updateView();
	}

	void UTIL::CAMERA::QuatCamera::zoom(const float z)
	{

		_position += _zaxis * z;
		
		
		updateView();

	}


	void UTIL::CAMERA::QuatCamera::updateView()
	{
		//Construct the view matrix from orientation quaternion and position vector

		//First get the matrix from the 'orientaation' Quaternion
		//This deals with the rotation and scale part of the view matrix
			_view = glm::mat4_cast(_orientation); // Rotation and Scale

		
		//Extract the camera coordinate axes from this matrix
		_xaxis = glm::vec3(_view[0][0], _view[1][0], _view[2][0]);
		_yaxis = glm::vec3(_view[0][1], _view[1][1], _view[2][1]);
		_zaxis = glm::vec3(_view[0][2], _view[1][2], _view[2][2]);

		//And use this and current camera position to set the translate part of the view matrix
		_view[3][0] = -glm::dot(_xaxis, _position); //Translation x
		_view[3][1] = -glm::dot(_yaxis, _position); //Translation y
		_view[3][2] = -glm::dot(_zaxis, _position); //Translation z
		
	}

	void UTIL::CAMERA::QuatCamera::Update(double deltaTime, glm::mat4 target)
	{
		if (m_follow == true)
		{
			m_carPos.x = target[3][0];
			m_carPos.y = target[3][1];
			m_carPos.z = target[3][2];

			_orientationTemp = glm::quat_cast(target);

			_projection = glm::perspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane);

			updateView();

		}
	}

	void UTIL::CAMERA::QuatCamera::Follow(double deltaTime, glm::vec3 carVelocity, glm::mat4 target, float angle)
	{
		
	}


	void UTIL::CAMERA::QuatCamera::roll(const float z)
	{

		glm::quat qzRoll = fromAxisAngle(WORLDZ, z);

		glm::quat zOutput = _orientation * qzRoll;

		glm::normalize(zOutput);

		_orientation = zOutput;

		updateView();

	}

	void UTIL::CAMERA::QuatCamera::reset(void)
	{
		//Initialise camera axes
		_xaxis = WORLDX;
		_yaxis = WORLDY;
		_zaxis = WORLDZ;

		//Initialise camera position 
		_position= glm::vec3(0.0f,2.0f,20.0f);

		//Initialise the orientation
		_orientation = glm::quat(1.0,0.0,0.0,0.0);

		//Initialise camera perspective parameters
		_fieldOfView = glm::radians(50.0f);
		_nearPlane = 0.01f;
		_farPlane = 1000.0f;
		_aspectRatio = 4.0f/3.0f;

        _projection = glm::perspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane);

		updateView();

	}


	glm::mat4 UTIL::CAMERA::QuatCamera::View()
	{
	
		this->updateView();
		return _view;
	}

	glm::mat4 UTIL::CAMERA::QuatCamera::Projection()
	{

		return _projection;
		
	}

