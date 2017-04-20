#pragma once

#include <memory.h>
#include "modelLoader.h"


namespace GRAPHICS {

	class RocketModel {

	public:
		/*! 
		\brief Constructor sets the model of the rocket to be used - flyweight 
		*/
		RocketModel(GRAPHICS::Model rocketModel) 
			: m_rocketModel(std::make_shared<GRAPHICS::Model>(rocketModel)) {};

		/*! 
		\brief getModel returns the currently set model.
		*/
		std::shared_ptr<GRAPHICS::Model> getModel() { return m_rocketModel; };

	private:
		std::shared_ptr<GRAPHICS::Model> m_rocketModel; //!< model to be used to render rockets.
	};
}