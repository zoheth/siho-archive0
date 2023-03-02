#pragma once

#include <utils/Entity.h>

#include <glm/glm.hpp>

namespace siho {

    class Engine;

    class Camera {
    public:
        Camera(Engine& engine, utils::Entity e);

        void terminate(Engine& engine) noexcept { }

        void setProjection(double fovInDegrees, double aspect, double near, double far);
    	void lookAt(glm::vec3 const& eye, double yaw, double pitch) noexcept;

        void update();

        glm::mat4 getViewMatrix() const noexcept;
        glm::mat4 getProjectionMatrix() const noexcept;

    private:
        Engine& mEngine;
        utils::Entity mEntity;

        glm::mat4 mProjection;
        glm::mat4 mProjectionForCulling;
        glm::vec3 mPosition;

        glm::vec3 mFront{0.0f,0.0f,-1.0f};
        glm::vec3 mUp{ 0.0f,0.0f,1.0f };
        glm::vec3 mRight{};
        glm::vec3 mWorldUp{ 0.0f,0.0f,1.0f };

    	double mZoom{};
        double mNear{};
        double mFar{};
        double mYaw{};
        double mPitch{};
    };
}
