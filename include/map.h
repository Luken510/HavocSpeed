#pragma once

#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>


#include "modelLoader.h"
#include "meshLoader.h"
#include "PhysicsController.h"
#include "shader.h"

#define MAP_SCALE 0.065f 
namespace GAME {

	class Map {

	public:
		/*!
		\brief Constructor
		*/
		Map();
		/*!
		\brief Initalise the Map
		*/
		void Init();
		/*!
		\brief Sets the current map
		*/
		void Update(double deltaTime);
		/*!
		\brief Render to render the map
		\param shader shader to render to
		*/
		void Render(std::shared_ptr<GRAPHICS::Shader> shader);
		/*!
		\brief CreateMapObjects initalises the map within bullet.
		*/
		void CreateMapObjects();

		/*!
		\brief UpdateMatrix function to update the model matrix of an enitity - used to help render models
		\param Pos position to set the model matrix to
		\param scale to scale the model matrix by
		*/
		void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale);
		/*!
		\brief UpdateMatrix function to update the model matrix of an enitity - used to help render models
		\param Pos position to set the matrix matrix to
		\param scale to scale the model matrix  by
		\param rotateAxis which axis to rotate current model matrix by
		\param angle angle to rotate the model matrix by
		*/
		void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale, glm::vec3 rotateAxis, float angle);
		/*!
		\brief UpdateMatrix function to update the matrix of an enitity - used to help render models
		\param matrix position and rotation to set the model matrix to
		\param scale to scale the model matrix by
		*/
		void UpdateMatrix(glm::mat4 matrix, glm::vec3 scale);

		/*!
		\brief GetTrackMatrix returns current track's matrix
		*/
		glm::mat4 GetTrackMatrix();
		/*!
		\brief GetWorldPos returns world position of track model within bullet
		*/
		glm::mat4 GetWorldPos();


	private:
		std::shared_ptr<GRAPHICS::Model> m_background = nullptr; //!< background model of the map
		std::shared_ptr<GRAPHICS::Model> m_track = nullptr; //!< the track
		std::vector<btConvexHullShape*> m_carTrackMesh; //!< btConvexHull shape of the track model

		btRigidBody* m_staticTrack = nullptr; //!< static RigidBody for the map

		glm::mat4 m_trackModelMatrix; //!< Track Model matrix
		glm::mat4 m_backgroundModelMatrix; //!< Track's background model matrix
	};
}