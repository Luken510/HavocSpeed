#pragma once
#ifndef QUATCAMERA_H
#define QUATCAMERA_H

#define _USE_MATH_DEFINES

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include "logger.h"

#define GLM_FORCE_RADIANS
#define PI 3.141592653589793
#define TWOPI 6.2831853071795862
#define TWOPI_F 6.2831853f
#define TO_RADIANS(x) (x * 0.017453292519943295)
#define TO_DEGREES(x) (x * 57.29577951308232)

namespace UTIL {
	namespace CAMERA {

		class QuatCamera
		{
		public:

			QuatCamera();  //Constructor
			QuatCamera(const glm::vec3& position);

			const glm::vec3& position() const; //position getter method
			void setPosition(const glm::vec3& position); //position setter method

			float fieldOfView() const; //fieldOfView getter method
			void setFieldOfView(float fieldOfView); //fieldOfView setter method

			float aspectRatio() const; //aspectRatio getter method
			void SetAspectRatio(float aspectRatio);  //aspectRatio setter method

			float nearPlane() const;  //nearPlane getter method
			float farPlane() const;   //farPlane getter method
			void setNearAndFarPlanes(float nearPlane, float farPlane); //nearPlane and farPLane setter method


			void rotate(const float yaw, const float pitch); //Rotate camera
			void pan(const float x, const float y);  //Pan camera
			void roll(const float z); //Roll camera
			void zoom(const float z); //Zoom camera


			void updateView();  //Update the camera

			void Update(double deltaTime, glm::mat4 target);

			void Follow(double deltaTime, glm::vec3 carVelocity, glm::mat4 target, float angle);

			void reset(void); //Reset the camera


			glm::mat4 View(); //Get the View matrix
			glm::mat4 G, Projection(); //Get the Projection matrix

	void Follow(glm::mat4 target, float angle);



		private:

			float _fieldOfView;
			float _nearPlane;
			float _farPlane;
			float _aspectRatio;

			//The camera coordinate axes
			glm::vec3 _xaxis;
			glm::vec3 _yaxis;
			glm::vec3 _zaxis;

			//Camera position vector and Quaternoin to represent camera orientation
			glm::vec3 _position;
			glm::quat _orientation;
			glm::quat _orientationTemp;

			glm::mat4 _view;
			glm::mat4 _projection;

			//World coordinate System Axes
			const glm::vec3 WORLDX = glm::vec3(1, 0, 0);
			const glm::vec3 WORLDY = glm::vec3(0, 1, 0);
			const glm::vec3 WORLDZ = glm::vec3(0, 0, 1);

			bool m_follow;
			bool m_turning;
			glm::vec3 m_carPos;
		};

	}
}

#endif


