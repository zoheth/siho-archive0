#include <glm/gtc/matrix_transform.hpp>
#include <siho/Camera.h>

siho::Camera::Camera(Engine& engine, utils::Entity e)
	: mEngine(engine), mEntity(e)
{

}

void siho::Camera::setProjection(double fovInDegrees, double aspect, double near, double far)
{
	mNear = near;
	mFar = far;
	mZoom = fovInDegrees;
	mProjection=glm::perspective(glm::radians(mZoom), aspect, near, far);
}

void siho::Camera::lookAt(glm::vec3 const& eye, double yaw, double pitch) noexcept: mPosition(eye), mYaw(yaw), mPitch(pitch)
{
	
}
