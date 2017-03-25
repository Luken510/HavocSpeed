#pragma once

#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>


#include "modelLoader.h"
#include "meshLoader.h"
#include "PhysicsController.h"
#include "shader.h"

#define MAP_SCALE 0.1f 
namespace GAME {

	class Map {

	public:
		Map();
		void Init();
		Map GetCurrentMap();
		void SetMap();
		void Update(double deltaTime);
		void Render(std::shared_ptr<GRAPHICS::Shader> shader);
		void CreateMapObjects();
		void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale);
		void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale, glm::vec3 rotateAxis, float angle);
		void UpdateMatrix(glm::mat4 matrix, glm::vec3 scale);
		glm::mat4 GetTrackMatrix();
		glm::mat4 GetWorldPos();


	private:
		std::shared_ptr<GRAPHICS::Model> m_background = nullptr;
		std::shared_ptr<GRAPHICS::Model> m_track = nullptr;

		btCollisionShape* m_carTrackMesh = nullptr;
		btCollisionShape* m_carBGMesh = nullptr;
		btRigidBody* m_staticTrack = nullptr;

		glm::mat4 m_trackModelMatrix;
	};
}