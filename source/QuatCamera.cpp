
#include <cmath>
#include "QuatCamera.h"
#include <btBulletDynamicsCommon.h>

#include <iostream>






	UTIL::QuatCamera::QuatCamera() : m_follow(false), m_turning(false)
	{
		reset();
	}

	UTIL::QuatCamera::QuatCamera(const glm::vec3& position) : m_follow(false), m_turning(false)
	{
		reset();
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

		glm::quat qyPitch = fromAxisAngle(WORLDX, pitch);
		glm::quat qxYaw = fromAxisAngle(WORLDY, yaw);

		glm::quat qOutput = qyPitch * _orientation * qxYaw; //pitch is effected by the local variables (_orientation)
			// yaw we want only affected by world co-ords which means we dont want it effect by orient i.e is doesnt use them (opengl goes backwards remember.)
		
		glm::normalize(qOutput);

		_orientation = qOutput;

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
		_xaxis = glm::vec3(_view[0][0], _view[1][0], _view[2][0]);
		_yaxis = glm::vec3(_view[0][1], _view[1][1], _view[2][1]);
		_zaxis = glm::vec3(_view[0][2], _view[1][2], _view[2][2]);

		//And use this and current camera position to set the translate part of the view matrix
		_view[3][0] = -glm::dot(_xaxis, _position); //Translation x
		_view[3][1] = -glm::dot(_yaxis, _position); //Translation y
		_view[3][2] = -glm::dot(_zaxis, _position); //Translation z
		
	}

	void UTIL::QuatCamera::update(glm::mat4 target)
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

	void UTIL::QuatCamera::Follow(glm::mat4 target, float angle)
	{
		glm::vec3 targetv3;
		targetv3.x = target[3][0];
		targetv3.y = target[3][1];
		targetv3.z = target[3][2];

		
		_orientationTemp = glm::quat_cast(target);

	//	setFieldOfView(angle);
		_position.x = targetv3.x;
		_position.y = targetv3.y + 100;
		_position.z = targetv3.z + 20;
		m_carPos = _position;


	//	_projection = glm::perspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane);

		m_follow = true;

		updateView();
	}

	void UTIL::QuatCamera::CarTurnLeft()
	{
		float cameraAngle = glm::angle(_orientation);
		float angle = glm::angle(_orientationTemp);
		UTIL::LOG(UTIL::LOG::INFO) << "Current angle turning for the camera = " << angle;
		UTIL::LOG(UTIL::LOG::INFO) << "Current angle of the camera = " << cameraAngle;

		if (cameraAngle < 1.0f)
		{
			cameraAngle = 1.0f;
		}
		if (cameraAngle > 3.0f)
		{
			cameraAngle = 3.0f;
		}

		glm::quat cameraRotationToCarY = fromAxisAngle(WORLDY, -angle*0.01f);

		glm::quat cameraRotationOutput = cameraRotationToCarY * _orientation;

		glm::normalize(cameraRotationOutput);

		_orientation.x = cameraRotationOutput.x;
		_orientation.y = cameraRotationOutput.y;
		_orientation.z = cameraRotationOutput.z;
		_orientation.w = cameraRotationOutput.w;
	}

	void UTIL::QuatCamera::CarDriving()
	{
		_position.x += _position.x - m_carPos.x * 0.001f;
		_position.y += _position.x - m_carPos.x * 0.001f;
		_position.z += _position.x - m_carPos.x * 0.001f;
		updateView();
	}

	void UTIL::QuatCamera::CarReversing()
	{
		_position.x += _position.x - m_carPos.x * 0.001f;
		_position.y += _position.y - m_carPos.y * 0.001f;
		_position.z += _position.z - m_carPos.z * 0.001f;

		updateView();
	}


	/*// Returns a quaternion such that q*start = dest
	glm::quat  UTIL::QuatCamera::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
		start = glm::normalize(start);
		dest = glm::normalize(dest);

		float cosTheta = glm::dot(start, dest);
		glm::vec3 rotationAxis;

		if (cosTheta < -1 + 0.001f) {
			// special case when vectors in opposite directions :
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			// This implementation favors a rotation around the Up axis,
			// since it's often what you want to do.
			rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
			if (glm::length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
				rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

			rotationAxis = glm::normalize(rotationAxis);
			return glm::angleAxis(180.0f, rotationAxis);
		}

		// Implementation from Stan Melax's Game Programming Gems 1 article
		rotationAxis = glm::cross(start, dest);

		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return glm::quat(
			s * 0.5f,
			rotationAxis.x * invs,
			rotationAxis.y * invs,
			rotationAxis.z * invs
		);


	}



	// Returns a quaternion that will make your object looking towards 'direction'.
	// Similar to RotationBetweenVectors, but also controls the vertical orientation.
	// This assumes that at rest, the object faces +Z.
	// Beware, the first parameter is a direction, not the target point !
	glm::quat  UTIL::QuatCamera::LookAt(glm::vec3 direction, glm::vec3 desiredUp) {

		if (glm::length2(direction) < 0.0001f)
			return glm::quat();

		// Recompute desiredUp so that it's perpendicular to the direction
		// You can skip that part if you really want to force desiredUp
		glm::vec3 right = glm::cross(direction, desiredUp);
		desiredUp = glm::cross(right, direction);

		// Find the rotation between the front of the object (that we assume towards +Z,
		// but this depends on your model) and the desired direction
		glm::quat rot1 = RotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), direction);
		// Because of the 1rst rotation, the up is probably completely screwed up. 
		// Find the rotation between the "up" of the rotated object, and the desired up
		glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
		glm::quat rot2 = RotationBetweenVectors(newUp, desiredUp);

		// Apply them
		return rot2 * rot1; // remember, in reverse order.
	}*/

	void UTIL::QuatCamera::CarTurnRight()
	{		
		float cameraAngle = glm::angle(_orientation);
		float angle = glm::angle(_orientationTemp);
		UTIL::LOG(UTIL::LOG::INFO) << "Current angle turning for the camera = " << angle;
		UTIL::LOG(UTIL::LOG::INFO) << "Current angle of the camera = " << cameraAngle;

		if (cameraAngle < 1.0f)
		{
			cameraAngle = 1.0f;
		}
		if (cameraAngle > 3.0f)
		{
			cameraAngle = 3.0f;
		}
		
			glm::quat cameraRotationToCarY = fromAxisAngle(WORLDY, angle*0.01f);

			glm::quat cameraRotationOutput = cameraRotationToCarY * _orientation;

			glm::normalize(cameraRotationOutput);

			_orientation.x = cameraRotationOutput.x;
			_orientation.y = cameraRotationOutput.y;
			_orientation.z = cameraRotationOutput.z;
			_orientation.w = cameraRotationOutput.w;
		
			

		
	}
