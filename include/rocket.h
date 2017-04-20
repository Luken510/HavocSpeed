#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>
#include "modelLoader.h"
#include "meshLoader.h"
#include "rocketModel.h"



namespace WEAPONS {
	class Rocket {

	public:
		/*
		\brief Constructor
		*/
		Rocket();
		Rocket(bool flyweight);
		/*
		\brief Destructor
		*/
		~Rocket();

		void Init();

		void CreateRocket();

		 void Update(double deltaTime);
		 void Render(std::shared_ptr<GRAPHICS::Shader> shader);

		 /*!
		 \brief GetMatrix  function to retreive an entities matrix.
		 */
		 glm::mat4 GetMatrix();

		 /*!
		 \brief GetWorldPos  function to retreive an entities world position.
		 */
		 glm::mat4 GetWorldPos();

		 /*!
		 \brief GetVelocity  function to retreive an entities Velocity.
		 */
		 glm::vec3 GetVelocity();


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
		 \brief Fire initalise fire sequence
		 \param carPosition to set the rocket relative to car position
		 */
		void Fire(glm::mat4 carPosition);

		/*!
		\brief GetRotationQuatFromAngle function to retrieve a rotate in quaternions from an axis and angle
		\param axis the axis to rotate by
		\param angle the angle to rotate by
		*/
		btQuaternion GetRotationQuatFromAngle(const btVector3& axis, btScalar angle);

		/*!
		\brief setModel function to set the rockets model - used in flyweight initalise
		\param rocketModel the RocketModel class to render the rockets model.
		*/
		void setModel(std::shared_ptr<GRAPHICS::RocketModel> rocketModel);


	private:

		std::shared_ptr<GRAPHICS::Model> m_rocket = nullptr; //!< rocket's model - used only in non flyweight mode
		std::shared_ptr<GRAPHICS::RocketModel> m_rocketModel = nullptr; //!< RocketModel class to init rocket model - used in flyweight mode
		glm::mat4 m_rocketMatrix; //!< rocket's model matrix.

		float m_damge; //!< damage dealt
		float m_velocity; //!< how fast the rocket goes
		bool m_rocketFired; //!< bool whether the rocket has already fired
		bool m_flyweight; //!< bool for flyweight mode
	};
}