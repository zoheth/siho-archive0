#pragma once

#include <glm/glm.hpp>
#include <utils/Entity.h>

namespace siho {

    class Engine;

    class Camera {
    public:
        Camera(Engine& engine, utils::Entity e);

        void terminate(Engine& engine) noexcept { }

        void setProjection(double fovInDegrees, double aspect, double near, double far);
        void lookAt(glm::vec3 const& eye, double yaw, double pitch) noexcept;

        glm::mat4 getViewMatrix() const noexcept;

    private:
        Engine& mEngine;
        utils::Entity mEntity;

        glm::mat4 mProjection;
        glm::mat4 mProjectionForCulling;
        glm::vec3 mPosition;
        double mZoom{};
        double mNear{};
        double mFar{};
        double mYaw{};
        double mPitch{};
    };
}
