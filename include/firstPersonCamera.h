#pragma once

#include "gl.hpp"
#include <glm\glm.hpp>
#include "cameraBase.h"
#include "raceCar.h"


namespace UTIL { namespace CAMERA {

		class firstPersonCamera : public CameraBase {
		public:
			/*!
			\brief Constructor
			*/
			firstPersonCamera();
			/*!
			\brief Constructor
			\param windowHeight the current windows height
			\param windowWidth the current windows width
			*/
			firstPersonCamera(float windowHeight, float windowWidth);
			/*!
			\brief Destructor
			*/
			~firstPersonCamera();

			/*!
			\brief Update a virtual function for updating the cameras position
			*/
			virtual void Update(double deltaTime);
			/*!
			\brief Follow a virtual function for updating the cameras position based on the car's position
			\param deltatime current time in frame
			\param carVelocity car's velocity
			\param target the targets current orientation
			*/
			void Follow(double deltaTime, glm::vec3 carVelocity, glm::mat4 target);

			/*!
			\brief GetView a virtual function for getting the camera view
			*/
			virtual glm::mat4 GetView();
			/*!
			\brief GetProjection a virtual function for getting the camera projection
			*/
			virtual glm::mat4 GetProjection();

			/*!
			\brief SetAspectRatio sets the aspect ratio
			*/
			virtual void SetAspectRatio(float aspectRatio);

		private:

			float m_windowHeight; //!< windows height
			float m_windowWidth; //!< windows width
			float m_nearPlane; //!< minimum render distance
			float m_farPlane; //!< maximum render distance
			float m_aspectRatio; //!< current aspectratio

			bool m_following; //!< boolean to determine if the camera is following an object

			glm::mat4 m_view; //!< camera's view
			glm::mat4 m_projection; //!< camera's projection
		};
	}
}