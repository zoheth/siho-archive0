#pragma once
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
constexpr double kYaw = -90.0;
constexpr double kPitch = 0.0;
constexpr double kSpeed = 1.2;
constexpr double kSensitivity = 0.03;
constexpr double kZoom = 45.0;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), double yaw = kYaw, double pitch = kPitch);
    Camera(double pos_x, double pos_y, double pos_z, double up_x, double up_y, double up_z, double yaw, double pitch);

    void SetUniforms(UniformObject& uniform_object) const;

    void SetProjection(double aspect, double near, double far);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const { return projection_; }

    void ProcessKeyboard(CameraMovement direction, double delta_time);

    void ProcessMouseMovement(double x_offset, double y_offset, bool constrain_pitch = true);

    void ProcessMouseScroll(double y_offset);

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
    double yaw_;
    double pitch_;
    // camera options
    double movement_speed_;
    double mouse_sensitivity_;
    double zoom_;
};