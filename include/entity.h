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

		/*!
		\brief GetMatrix virtual function to retreive an entities matrix.
		*/
		virtual glm::mat4 GetMatrix() = 0;

		/*!
		\brief GetWorldPos virtual function to retreive an entities world position.
		*/
		virtual glm::mat4 GetWorldPos() = 0;

		/*!
		\brief GetVelocity virtual function to retreive an entities Velocity.
		*/
		virtual	glm::vec3 GetVelocity() = 0;

		/*!
		\brief GetRotationQuatFromAngle function to retrieve a rotate in quaternions from an axis and angle
		\param axis the axis to rotate by
		\param angle the angle to rotate by
		*/
		virtual btQuaternion GetRotationQuatFromAngle(const btVector3& axis, btScalar angle) = 0;


		/*!
		\brief UpdateMatrix function to update the model matrix of an enitity - used to help render models
		\param Pos position to set the model matrix to
		\param scale to scale the model matrix by
		*/
		virtual void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale) = 0;
		/*!
		\brief UpdateMatrix function to update the model matrix of an enitity - used to help render models
		\param Pos position to set the matrix matrix to
		\param scale to scale the model matrix  by
		\param rotateAxis which axis to rotate current model matrix by
		\param angle angle to rotate the model matrix by
		*/
		virtual void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale, glm::vec3 rotateAxis, float angle) = 0;

		/*!
		\brief UpdateMatrix function to update the matrix of an enitity - used to help render models
		\param matrix position and rotation to set the model matrix to
		\param scale to scale the model matrix by
		*/
		virtual void UpdateMatrix(glm::mat4 matrix, glm::vec3 scale) = 0;


		/*!
		\brief Update Update the Entity, main game logic function
		\param deltaTime current time at the frame.
		*/
		virtual void Update(double deltaTime) = 0;

		/*!
		\brief Render rendering the object using a shader
		\param shader the shader to render to
		*/
		virtual void Render(std::shared_ptr<GRAPHICS::Shader> shader) = 0;


		/*!
		\brief Init initialise the Entity.
		*/
		virtual void Init() = 0;
	};
}