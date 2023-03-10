#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <siho/Renderer.h>
#include <siho/MeshAssimp.h>
#include <siho/Scene.h>
#include <siho/Camera.h>

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
	mesh_assimp.LoadModel("C:\\3D Models\\Helmet\\untitled.gltf");

	Scene scene;
	scene.set_renderables(mesh_assimp.renderables());

	scene.AddLight(PointLight{
		glm::vec3(0.0f, 10.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		250.0f });
	Shader scene_shader("shaders\\common.vs", "shaders\\common.fs");

	Shader process_shader("shaders\\post_processing.vs", "shaders\\post_processing.fs");
	Shader blur_shader("shaders\\post_processing.vs", "shaders\\blur.fs");
	Shader blend_shader("shaders\\post_processing.vs", "shaders\\blend.fs");

	scene_shader.use();
	auto model = glm::mat4(1.0f);
	//model = glm::scale(model, glm::vec3(100.0f));
	scene_shader.setMat4("model", model);

	Camera camera;
	camera.SetProjection(static_cast<float>(width) / height, 0.1f, 100.0f);
	UniformObject uniform_camera;

	window.SetCamera(&camera);


	RenderTarget scene_render_target(width, height, 2);
	RenderTarget blurRenderTarget(width, height);
	RenderTarget bloomRenderTarget(width, height);

	SceneRenderPass scene_pass(&scene_render_target, scene_shader, scene);

	BlurPass blur_pass(&blurRenderTarget, blur_shader, scene_pass);

	/*BlendPass blend_pass(&blendRenderTarget, blend_shader);
	blend_pass.AddTexture(scene_pass.target()->color_buffer());
	blend_pass.AddTexture(blur_pass.target()->color_buffer());*/

	BloomPass bloom_pass(&bloomRenderTarget, blend_shader, blur_pass);

	FinalRenderPass final_pass(nullptr, process_shader, bloom_pass);


	glEnable(GL_DEPTH_TEST);
	do
	{
		window.ProcessContext();

		window.ShowLightEditorWindow(scene);

		camera.SetUniforms(uniform_camera);
		uniform_camera.apply(scene_shader);

		//scene_pass.render();
		final_pass.render();

		window.DrawUi();

		window.SwapBuffers();

	} while (window.ShouldClose());

	//ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();
	glfwTerminate();

	return 0;

}

