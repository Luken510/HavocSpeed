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

		void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale);
		void UpdateMatrix(glm::vec3 Pos, glm::vec3 scale, glm::vec3 rotateAxis, float angle);
		void UpdateMatrix(glm::mat4 matrix, glm::vec3 scale);
		 glm::mat4 GetMatrix();
		 glm::mat4 GetWorldPos();
		 glm::vec3 GetVelocity();

		void Fire(glm::mat4 carPosition);

		btQuaternion GetRotationQuatFromAngle(const btVector3& axis, btScalar angle);

		void setModel(std::shared_ptr<GRAPHICS::RocketModel> rocketModel);


	private:

		std::shared_ptr<GRAPHICS::Model> m_rocket = nullptr;
		std::shared_ptr<GRAPHICS::RocketModel> m_rocketModel = nullptr;
		glm::mat4 m_rocketMatrix;

		float m_power;
		float m_damge;
		float m_velocity;
		bool m_rocketFired;
		bool m_flyweight;
	};
}