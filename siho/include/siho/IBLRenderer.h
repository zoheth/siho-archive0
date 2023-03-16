#pragma once
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "RenderTarget.h"

class Camera;

class IBLRenderer
{
public:
	explicit IBLRenderer(const std::string& hdr_image_path, Camera& camera);

    void RenderSkybox() const;

	[[nodiscard]]unsigned int irradiance_map() const { return irradiance_map_; }
	[[nodiscard]]unsigned int prefilter_map() const { return prefilter_map_; }
	[[nodiscard]]unsigned int brdf_lut_texture() const { return brdf_lut_texture_; }
private:
    void LoadHdrImage(const std::string& hdr_image_path);
    void CreateEnvCubemap();
    void CreateIrradianceMap();
    void CreatePrefilterMap();

    Camera& camera_;
    Shader to_cubemap_shader_{ "shaders\\cubemap.vs", "shaders\\equirectangular_to_cubemap.fs" };
    Shader irradiance_shader_{ "shaders\\cubemap.vs", "shaders\\irradiance_convolution.fs" };
    Shader prefilter_shader_{ "shaders\\cubemap.vs", "shaders\\prefilter.fs" };
    Shader brdf_shader_{ "shaders\\brdf.vs", "shaders\\brdf.fs" };
    Shader background_shader_{ "shaders\\background.vs", "shaders\\background.fs" };
    RenderTarget capture_target_{ 512,512,0,1 };
    unsigned int hdr_texture_{};
    unsigned int env_cubemap_{};
    unsigned int irradiance_map_{};
    unsigned int prefilter_map_{};
    unsigned int brdf_lut_texture_{};

    unsigned int capture_fbo_{};
    unsigned int capture_rbo_{};

    glm::mat4 capture_projection_{};
    glm::mat4 capture_views_[6]{ glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)) };

};