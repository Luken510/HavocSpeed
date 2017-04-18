#include "rocket.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

WEAPONS::Rocket::Rocket() :m_flyweight(false)
{
	//without flyweight;
	m_rocket = std::make_shared<GRAPHICS::Model>("./external/assets/weapons/Missile/AVMT300/AVMT300.3ds");
	//randmise spawn
	int randomiseX = rand() % 50 + 30;
	// set to random x and z, not y though - just for testing, would be set to the cars matrix + a bit.
	int randomiseZ = rand() % 50 + 30;
	m_rocketMatrix = glm::mat4(1.0f) * glm::translate(glm::vec3(randomiseX, 80, randomiseZ));

}

WEAPONS::Rocket::Rocket(bool flyweight)
{
	m_flyweight = flyweight;
	if (m_flyweight) {
		//randmise spawn
		int randomiseX = rand()% 50 + 30; 
		// set to random x and z, not y though - just for testing, would be set to the cars matrix + a bit.
		int randomiseZ = rand() % 50 + 30;
		m_rocketMatrix = glm::mat4(1.0f) * glm::translate(glm::vec3(randomiseX, 80, randomiseZ));
	};
}

WEAPONS::Rocket::~Rocket()
{
}

void WEAPONS::Rocket::Init()
{
	
}

void WEAPONS::Rocket::CreateRocket()
{
	//bullet interrgration
}

void WEAPONS::Rocket::Update(double deltaTime)
{

}
void WEAPONS::Rocket::Render(std::shared_ptr<GRAPHICS::Shader> shader)
{
	//UpdateMatrix(GetWorldPos(), glm::vec3(ROCKET_SCALE));
	if (m_flyweight)
	{
		shader->SetUniform("model", m_rocketMatrix);
		m_rocketModel->getModel()->Render(shader);
	}
	else
	{
		shader->SetUniform("model", m_rocketMatrix);
		m_rocket->Render(shader);
	}
}

void WEAPONS::Rocket::UpdateMatrix(glm::vec3 Pos, glm::vec3 scale)
{
	m_rocketMatrix = glm::translate(Pos) * glm::scale(scale);
}

void WEAPONS::Rocket::UpdateMatrix(glm::vec3 Pos, glm::vec3 scale, glm::vec3 rotateAxis, float angle)
{
	m_rocketMatrix = glm::translate(Pos) * glm::rotate(glm::radians(angle), rotateAxis) * glm::scale(scale);
}

void WEAPONS::Rocket::UpdateMatrix(glm::mat4 matrix, glm::vec3 scale)
{
	m_rocketMatrix = matrix * glm::scale(scale); // loses scale so re-adding
}

glm::mat4 WEAPONS::Rocket::GetMatrix()
{
	return glm::mat4();
}

glm::mat4 WEAPONS::Rocket::GetWorldPos()
{
	return glm::mat4();
}

glm::vec3 WEAPONS::Rocket::GetVelocity()
{
	return glm::vec3();
}

void WEAPONS::Rocket::Fire(glm::mat4 carPosition)
{
}

btQuaternion WEAPONS::Rocket::GetRotationQuatFromAngle(const btVector3 & axis, btScalar angle)
{
	btQuaternion rotation;

	rotation.setRotation(axis, btRadians(angle));

	return rotation;
}

void WEAPONS::Rocket::setModel(std::shared_ptr<GRAPHICS::RocketModel> rocketModel)
{
	m_rocketModel = rocketModel;
}
