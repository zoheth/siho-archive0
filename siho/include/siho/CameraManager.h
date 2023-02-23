#pragma once
#include "Camera.h"

namespace siho
{
	class CameraManager
	{
	public:
		explicit CameraManager(Engine& engine);
		Camera* create(utils::Entity entity);
	private:
		Engine& mEngine;
	};

}
