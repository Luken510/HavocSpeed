#pragma once

#include <glm\glm.hpp>
#include "logger.h"
#define GLM_FORCE_RADIANS
namespace UTIL { namespace CAMERA {

		class CameraBase {

		public:
			/*!
			\brief virtual function update
			*/
			virtual void Update(double deltaTime) { UTIL::LOG(UTIL::LOG::INFO) << "Base Camera Function Called "; };
			/*!
			\brief virtual function GetProjection
			*/
			virtual glm::mat4 GetProjection() { UTIL::LOG(UTIL::LOG::INFO) << "Base Camera Function Called "; return glm::mat4(1.0f); };
			/*!
			\brief virtual function GetView
			*/
			virtual glm::mat4 GetView() { UTIL::LOG(UTIL::LOG::INFO) << "Base Camera Function Called "; return glm::mat4(1.0f);};
			/*!
			\brief virtual function SetAspectRatio
			*/
			virtual void SetAspectRatio(float aspectRatio) { UTIL::LOG(UTIL::LOG::INFO) << "Base Camera Function Called "; };
			/*!
			\brief virtual function zoom
			*/
			virtual void zoom(const float z)  { UTIL::LOG(UTIL::LOG::INFO) << "Base Camera Function Called "; };
			 /*!
			 \brief virtual function Follow current target
			 */
			virtual void Follow(double deltaTime, glm::vec3 carVelocity, glm::mat4 target) = 0 { UTIL::LOG(UTIL::LOG::INFO) << "Base Camera Function Called "; };

		};
	}
}