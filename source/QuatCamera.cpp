
#include <cmath>
#include "QuatCamera.h"

#include <iostream>






	UTIL::QuatCamera::QuatCamera() 
	{
		reset();
		int iCurrentCamera = 1;
	}

	UTIL::QuatCamera::QuatCamera(const glm::vec3& position)
	{
		reset();
		int iCurrentCamera = 1;
		_position = position;
	}


	const glm::vec3& UTIL::QuatCamera::position() const
	{
		return _position;
	}

	void UTIL::QuatCamera::setPosition(const glm::vec3& position)
	{
		_position = position;
	}



	float UTIL::QuatCamera::fieldOfView() const
	{
		return _fieldOfView;
	}


	void UTIL::QuatCamera::setFieldOfView(float fieldOfView)
	{
		assert(fieldOfView>0.0f && fieldOfView <180.0f);
		_fieldOfView = fieldOfView;
	}


	float UTIL::QuatCamera::aspectRatio() const
	{
		return _aspectRatio;
	}

	void UTIL::QuatCamera::setAspectRatio(float aspectRatio)
	{
		assert(aspectRatio >0.0f);
		_aspectRatio = aspectRatio;
	}


	float UTIL::QuatCamera::nearPlane() const
	{
		return _nearPlane;
	}

	float UTIL::QuatCamera::farPlane() const
	{
		return _farPlane;
	}

	

	void UTIL::QuatCamera::setNearAndFarPlanes(float nearPlane, float farPlane)
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
	void UTIL::QuatCamera::rotate(const float yaw, const float pitch)
	{

		glm::quat qxPitch = fromAxisAngle(WORLDX, pitch);
		glm::quat qyYaw = fromAxisAngle(WORLDY, yaw);

		glm::quat qOutput = qxPitch * _orientation * qyYaw; //pitch is effected by the local variables (_orientation)
			// yaw we want only affected by world co-ords which means we dont want it effect by orient i.e is doesnt use them (opengl goes backwards remember.)
		
		glm::normalize(qOutput);

		_orientation = qOutput;

		//if (iCurrentCamera == 1)
			//std::cout << "1" << std::endl;
		//else
			//std::cout << "2" << std::endl;


		updateView();

	}

	
	void UTIL::QuatCamera::pan(const float x, const float y)
	{
		
		_position += _xaxis * x;
		_position += _yaxis * -y;
		
		
		updateView();
	}

	void UTIL::QuatCamera::zoom(const float z)
	{

		_position += _zaxis * z;
		
		
		updateView();

	}


	void UTIL::QuatCamera::updateView()
	{
		//Construct the view matrix from orientation quaternion and position vector

		//First get the matrix from the 'orientaation' Quaternion
		//This deals with the rotation and scale part of the view matrix
		_view = glm::mat4_cast(_orientation); // Rotation and Scale

		//Extract the camera coordinate axes from this matrix
		_xaxis = glm::vec3(_view[0][0],_view[1][0],_view[2][0]);
		_yaxis = glm::vec3(_view[0][1],_view[1][1],_view[2][1]);
		_zaxis = glm::vec3(_view[0][2],_view[1][2],_view[2][2]);
		
		//And use this and current camera position to set the translate part of the view matrix
		_view[3][0] = -glm::dot(_xaxis,_position); //Translation x
		_view[3][1] = -glm::dot(_yaxis,_position); //Translation y
		_view[3][2] = -glm::dot(_zaxis,_position); //Translation z
	
	}


	void UTIL::QuatCamera::roll(const float z)
	{

		glm::quat qzRoll = fromAxisAngle(WORLDZ, z);

		glm::quat zOutput = _orientation * qzRoll;

		glm::normalize(zOutput);

		_orientation = zOutput;

		updateView();

	}

	void UTIL::QuatCamera::reset(void)
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

		_projection = glm::perspective(_fieldOfView,_aspectRatio,_nearPlane,_farPlane);

		updateView();
	}


	glm::mat4 UTIL::QuatCamera::view()
	{
	
		this->updateView();
		return _view;
	}

	glm::mat4 UTIL::QuatCamera::projection()
	{

		return _projection;
		
	}

	void UTIL::QuatCamera::toggleCamera(int x)
	{
		if (x == 1)
		{
			iCurrentCamera = 1;
		}
		else if (x == 2)
		{
			iCurrentCamera = 2;
		}
			
	}

	int UTIL::QuatCamera::getCurrentCam()
	{
		return iCurrentCamera;
	}