#pragma once

#include <memory.h>
#include "modelLoader.h"


namespace GRAPHICS {

	class RocketModel {

	public:
		RocketModel(GRAPHICS::Model rocketModel) 
			: m_rocketModel(std::make_shared<GRAPHICS::Model>(rocketModel)) {};

		std::shared_ptr<GRAPHICS::Model> getModel() { return m_rocketModel; };

	private:
		std::shared_ptr<GRAPHICS::Model> m_rocketModel;
	};
}