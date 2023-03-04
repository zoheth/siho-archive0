#include <siho/Renderer.h>
#include <siho/MeshAssimp.h>
#include <siho/Scene.h>
#include <siho/Camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "sihoapp/Window.h"


using namespace siho;


int main(void)
{
	int width = 1920, height = 1080;
	Window window(width, height, "Siho");

	MeshAssimp mesh_assimp;
	//mesh_assimp.LoadModel("E:\\Study\\OpenGL\\Project0\\Models\\Girl\\untitled.obj.ass");
	mesh_assimp.LoadModel("C:\\3D Models\\lamp.gltf");

	Scene scene;
	scene.set_renderables(mesh_assimp.renderables());

	scene.AddLight(PointLight{
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f });
	Shader shader("shaders\\common.vs", "shaders\\common.fs");
	shader.use();

	auto model = glm::mat4(1.0f);
	//model = glm::scale(model, glm::vec3(100.0f));
	shader.setMat4("model", model);

	Camera camera;
	camera.SetProjection(static_cast<float>(width) / height, 0.1f, 100.0f);
	UniformObject uniform_camera;

	window.SetCamera(&camera);

	glEnable(GL_DEPTH_TEST);
	do
	{
		window.MakeContextCurrent();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//ImGui_ImplGlfw_NewFrame();

		camera.SetUniforms(uniform_camera);
		uniform_camera.apply(shader);

		scene.render(shader);

		window.SwapBuffers();
		window.PollEvents();

	} while (window.ShouldClose());

	//ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();
	glfwTerminate();

	return 0;

}

