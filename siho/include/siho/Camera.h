#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "UniformObject.h"

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
constexpr float kYaw = -90.0f;
constexpr float kPitch = 0.0f;
constexpr float kSpeed = 2.5f;
constexpr float kSensitivity = 0.05f;
constexpr float kZoom = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = kYaw, float pitch = kPitch);
    Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);

    void SetUniforms(UniformObject& uniform_object) const;

    void SetProjection(float aspect, float near, float far);

    glm::mat4 GetViewMatrix() const;

    void ProcessKeyboard(CameraMovement direction, float delta_time);

    void ProcessMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch = true);

    void ProcessMouseScroll(float y_offset);

private:
    void UpdateCameraVectors();
    // camera Attributes
    glm::mat4 projection_;
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;
    // euler Angles
    float yaw_;
    float pitch_;
    // camera options
    float movement_speed_;
    float mouse_sensitivity_;
    float zoom_;
};