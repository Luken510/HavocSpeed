#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletDynamics/Vehicle/btRaycastVehicle.h>
#include <BulletDynamics/ConstraintSolver/btHingeConstraint.h>
#include <BulletDynamics/ConstraintSolver/btSliderConstraint.h>

#include "shader.h"


namespace UTIL {

	class Entity {

	public:

		virtual glm::mat4 GetMatrix() = 0;
		virtual glm::mat4 GetWorldPos() = 0;
		virtual	glm::vec3 GetVelocity() = 0;
		virtual btQuaternion GetRotationQuatFromAngle(const btVector3& axis, btScalar angle) = 0;

		virtual void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale) = 0;
		virtual void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale, glm::vec3 rotateAxis, float angle) = 0;
		virtual void UpdateMatrix(glm::mat4 matrix, glm::vec3 scale) = 0;


		virtual void Update(double deltaTime) = 0;
		virtual void Render(std::shared_ptr<GRAPHICS::Shader> shader) = 0;

		virtual void Init() = 0;
	};
}