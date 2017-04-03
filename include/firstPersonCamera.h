#pragma once

#include "gl.hpp"
#include <glm\glm.hpp>
#include "cameraBase.h"
#include "raceCar.h"


namespace UTIL { namespace CAMERA {

		class firstPersonCamera : public CameraBase {
		public:
			firstPersonCamera();
			firstPersonCamera(float windowHeight, float windowWidth);
			~firstPersonCamera();

			virtual  void Update(double deltaTime);
			virtual void Follow(double deltaTime, glm::vec3 carVelocity, glm::mat4 target);

			virtual glm::mat4 GetView();
			virtual	glm::mat4 GetProjection();

			virtual void SetAspectRatio(float aspectRatio);

		private:

			float m_windowHeight;
			float m_windowWidth;
			float m_nearPlane;
			float m_farPlane;
			float m_aspectRatio;

			bool m_following;

			glm::mat4 m_view;
			glm::mat4 m_projection;
		};
	}
}