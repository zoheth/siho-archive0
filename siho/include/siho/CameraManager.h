#pragma once


namespace utils
{
	class Entity;
}

namespace siho
{
	class Camera;
	class Engine;
	class CameraManager
	{
	public:
		explicit CameraManager(Engine& engine);
		Camera* create(utils::Entity entity);
	private:
		Engine& mEngine;
	};

}
