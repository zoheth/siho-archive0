#include <siho/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, double yaw, double pitch): front_(glm::vec3(0.0f, 1.0f, -1.0f)), movement_speed_(kSpeed), mouse_sensitivity_(kSensitivity), zoom_(kZoom)
{
	position_ = position;
	world_up_ = up;
	yaw_ = yaw;
	pitch_ = pitch;
	UpdateCameraVectors();
}

Camera::Camera(double pos_x, double pos_y, double pos_z, double up_x, double up_y, double up_z, double yaw, double pitch): front_(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed_(kSpeed), mouse_sensitivity_(kSensitivity), zoom_(kZoom)
{
	position_ = glm::vec3(pos_x, pos_y, pos_z);
	world_up_ = glm::vec3(up_x, up_y, up_z);
	yaw_ = yaw;
	pitch_ = pitch;
	UpdateCameraVectors();
}

void Camera::SetUniforms(UniformObject& uniform_object) const
{
	uniform_object.set(uniforms::kView, glm::lookAt(position_, position_ + front_, up_));
	uniform_object.set(uniforms::kProjection, projection_);
	uniform_object.set(uniforms::kCameraPosition, position_);
}

void Camera::SetProjection(const double aspect, const double near, const double far)
{
	projection_ = glm::perspective(glm::radians(zoom_), aspect, near, far);
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::ProcessKeyboard(const CameraMovement direction, const double delta_time)
{
	const float velocity = movement_speed_ * delta_time;
	if (direction == FORWARD)
		position_ += front_ * velocity;
	if (direction == BACKWARD)
		position_ -= front_ * velocity;
	if (direction == LEFT)
		position_ -= right_ * velocity;
	if (direction == RIGHT)
		position_ += right_ * velocity;
}

void Camera::ProcessMouseMovement(double x_offset, double y_offset, bool constrain_pitch)
{
	x_offset *= mouse_sensitivity_;
	y_offset *= mouse_sensitivity_;

	yaw_ += x_offset;
	pitch_ += y_offset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrain_pitch)
	{
		if (pitch_ > 89.0)
			pitch_ = 89.0;
		if (pitch_ < -89.0)
			pitch_ = -89.0;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(double y_offset)
{
	zoom_ -= y_offset;
	if (zoom_ < 1.0)
		zoom_ = 1.0;
	if (zoom_ > 45.0)
		zoom_ = 45.0;
}

void Camera::UpdateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front.y = sin(glm::radians(pitch_));
	front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_ = glm::normalize(front);
	// also re-calculate the Right and Up vector
	right_ = glm::normalize(glm::cross(front_, world_up_));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up_ = glm::normalize(glm::cross(right_, front_));
}
