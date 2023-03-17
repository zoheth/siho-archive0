#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <siho/Renderer.h>
#include <siho/MeshAssimp.h>
#include <siho/Scene.h>
#include <siho/Camera.h>
#include <siho/IBLRenderer.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "siho/RenderPass.h"
#include "siho/RenderTarget.h"
#include "sihoapp/Window.h"


using namespace siho;


int main(void)
{
	int width = 1920, height = 1080;
	Window window(width, height, "Siho");

	MeshAssimp mesh_assimp;
	//mesh_assimp.LoadModel("E:\\Study\\OpenGL\\Project0\\Models\\Girl\\untitled.obj.ass");
	mesh_assimp.LoadModel("assets\\Helmet\\untitled.gltf");
	//mesh_assimp.LoadModel("C:\\3D Models\\lamp\\lamp.gltf");

	Scene scene;
	scene.set_renderables(mesh_assimp.renderables());

	scene.AddLight(PointLight{
		glm::vec3(2.0f, 3.0f, 2.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		200.0f });
	//Shader scene_shader("shaders\\common.vs", "shaders\\common.fs");
	Shader scene_shader("shaders\\common.vs", "shaders\\standard.fs");
	Shader shadow_shader("shaders\\shadow.vs", "shaders\\shadow.fs", "shaders\\shadow.gs");

	Shader process_shader("shaders\\post_processing.vs", "shaders\\post_processing.fs");
	Shader blur_shader("shaders\\post_processing.vs", "shaders\\blur.fs");
	Shader blend_shader("shaders\\post_processing.vs", "shaders\\blend.fs");

	scene_shader.use();
	auto model = glm::mat4(1.0f);
	//model = glm::scale(model, glm::vec3(100.0f));
	scene_shader.setMat4("model", model);

	shadow_shader.use();
	shadow_shader.setMat4("model", model);

	Camera camera;
	camera.SetProjection(static_cast<float>(width) / height, 0.1f, 100.0f);
	UniformObject uniform_camera;

	window.SetCamera(&camera);

	IBLRenderer ibl("assets\\hdr\\newport_loft.hdr", camera);

	RenderTarget scene_render_target(width, height, 2,1);
	RenderTarget blur_render_target(width, height, 1,1);
	RenderTarget bloom_render_target(width, height, 1,1);
	RenderTarget* shadow_target = RenderTarget::CreateShadowTarget(2048, 2048);

	SceneRenderPass scene_pass(&scene_render_target, scene_shader, scene);
	scene_pass.SetIblRenderer(&ibl);
	ShadowMapPass shadow_pass(shadow_target, shadow_shader, scene);
	scene_pass.SetShadowMapPass(&shadow_pass);

	BlurPass blur_pass(&blur_render_target, blur_shader, scene_pass);

	/*BlendPass blend_pass(&blendRenderTarget, blend_shader);
	blend_pass.AddTexture(scene_pass.target()->color_buffer());
	blend_pass.AddTexture(blur_pass.target()->color_buffer());*/

	BloomPass bloom_pass(&bloom_render_target, blend_shader, blur_pass);

	FinalRenderPass final_pass(nullptr, process_shader, &bloom_pass);



	bool enable_bloom = true;
	bool enable_ibl = true;
	bool enable_shadow = true;
	glEnable(GL_DEPTH_TEST);
	do
	{
		window.ProcessContext();

		window.ShowLightEditorWindow(scene);

		ImGui::Checkbox("Shadow", &enable_shadow);
		ImGui::Checkbox("Bloom", &enable_bloom);
		ImGui::Checkbox("IBL", &enable_ibl);
		if (enable_shadow) scene_pass.SetShadowMapPass(&shadow_pass);
		else scene_pass.SetShadowMapPass(nullptr);
		if (enable_bloom) final_pass.set_prev_pass(&bloom_pass);
		else final_pass.set_prev_pass(&scene_pass);
		if(enable_ibl) scene_pass.SetIblRenderer(&ibl);
		else scene_pass.SetIblRenderer(nullptr);

		camera.SetUniforms(uniform_camera);
		uniform_camera.apply(scene_shader);


		//scene_pass.render();
		final_pass.render();
		//ibl.RenderSkybox();

		window.DrawUi();

		window.SwapBuffers();

	} while (window.ShouldClose());

	//ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();
	glfwTerminate();

	return 0;

}

