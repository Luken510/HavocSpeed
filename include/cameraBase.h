#pragma once

#include <glm\glm.hpp>
#include "logger.h"
#define GLM_FORCE_RADIANS
namespace UTIL { namespace CAMERA {

		class CameraBase {

		public:
			virtual void Update(double deltaTime) { UTIL::LOG(UTIL::LOG::INFO) << "Base Camera Function Called "; };
			virtual glm::mat4 GetProjection() { UTIL::LOG(UTIL::LOG::INFO) << "Base Camera Function Called "; return glm::mat4(1.0f); };
			virtual glm::mat4 GetView() { UTIL::LOG(UTIL::LOG::INFO) << "Base Camera Function Called "; return glm::mat4(1.0f);};
			virtual void SetAspectRatio(float aspectRatio) { UTIL::LOG(UTIL::LOG::INFO) << "Base Camera Function Called "; };

			virtual void zoom(const float z)  { UTIL::LOG(UTIL::LOG::INFO) << "Base Camera Function Called "; }; //Zoom camera
			virtual void Follow(double deltaTime, glm::vec3 carVelocity, glm::mat4 target) = 0 { UTIL::LOG(UTIL::LOG::INFO) << "Base Camera Function Called "; };

		};
	}
}