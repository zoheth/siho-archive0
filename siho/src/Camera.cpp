#include <glm/gtc/matrix_transform.hpp>
#include <siho/Camera.h>

siho::Camera::Camera(Engine& engine, utils::Entity e)
	: mEngine(engine), mEntity(e)
{
	update();
}

void siho::Camera::setProjection(double fovInDegrees, double aspect, double near, double far)
{
	mNear = near;
	mFar = far;
	mZoom = fovInDegrees;
	mProjection=glm::perspective(glm::radians(mZoom), aspect, near, far);
}

void siho::Camera::lookAt(glm::vec3 const& eye, double yaw, double pitch) noexcept
{
	mPosition = eye;
	mYaw = yaw;
	mPitch = pitch;
}

void siho::Camera::update()
{
	glm::vec3 front;
	front.x = cos(glm::radians(mPitch)) * cos(glm::radians(mYaw));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);
	// also re-calculate the Right and Up vector
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	mUp = glm::normalize(glm::cross(mRight, mFront));
}

glm::mat4 siho::Camera::getViewMatrix() const noexcept
{
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

glm::mat4 siho::Camera::getProjectionMatrix() const noexcept
{
	return mProjection;
}
