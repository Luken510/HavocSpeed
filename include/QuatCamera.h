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
			/*!
			\brief Constructor
			*/
			QuatCamera();
			/*!
			\brief Constructor
			\param position camera's position
			*/
			QuatCamera(const glm::vec3& position);

			/*!
			\brief position getter method
			*/
			const glm::vec3& position() const; 
			/*!
			\brief position setter method
			\param position camera's position
			*/
			void setPosition(const glm::vec3& position);
			/*!
			\brief fieldOfView getter method
			*/
			float fieldOfView() const; 
			/*!
			\brief fieldOfView setter method
			\param fieldOfView camera's field Of View
			*/
			void setFieldOfView(float fieldOfView); 
			/*!
			\brief aspectRatio getter method
			*/
			float aspectRatio() const; 

			/*!
			\brief aspectRatio setter method
			\param aspectRatio camera's aspectRatio
			*/
			void SetAspectRatio(float aspectRatio);  

			/*!
			\brief nearPlane getter method
			*/
			float nearPlane() const;  
			/*!
			\brief farPlane getter method
			*/
			float farPlane() const; 
			/*!
			\brief nearPlane and farPLane setter method
			\param nearPlane cameras miniumum render
			\param farPlane cameras maximum render
			*/
			void setNearAndFarPlanes(float nearPlane, float farPlane); //

			/*!
			\brief Rotate rotate camera
			\param yaw rotate on the yaw axis
			\param pitch rotate on the pitch axis
			*/
			void rotate(const float yaw, const float pitch); 
			/*!
			\brief Pan pan the  camera
			\param x Pan on the yaw axis
			\param y Pan on the pitch axis
			*/
			void pan(const float x, const float y);  
			/*!
			\brief roll roll the  camera
			\param z amount to roll by
			*/
			void roll(const float z);
			/*!
			\brief zoom zoom the camera
			\param z amount to zoom in by
			*/
			void zoom(const float z); 

			/*!
			\brief updateView Update the camera view
			*/
			void updateView();  
			/*!
			\brief Update Update the camera view
			*/
			void Update(double deltaTime, glm::mat4 target);
			/*!
			\brief Follow follow set target
			\param carVelocity car's velocity
			\param target targets matrix
			\param angle angle to follow him by
			*/
			void Follow(double deltaTime, glm::vec3 carVelocity, glm::mat4 target, float angle);
			/*!
			\brief reset reset the current cammera
			*/
			void reset(void); 

			/*!
			\brief View return the current camera view
			*/
			glm::mat4 View(); 
			/*!
			\brief Projection return the projection
			*/
			glm::mat4 G, Projection(); 



		private:

			float m_fieldOfView;//<
			float m_nearPlane;
			float m_farPlane;
			float m_aspectRatio;

			//The camera coordinate axes
			glm::vec3 m_xaxis;
			glm::vec3 m_yaxis;
			glm::vec3 m_zaxis;

			//Camera position vector and Quaternoin to represent camera orientation
			glm::vec3 m_position;
			glm::quat m_orientation;
			glm::quat m_orientationTemp;

			glm::mat4 m_view;
			glm::mat4 m_projection;

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


