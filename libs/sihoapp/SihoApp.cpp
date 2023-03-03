#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <siho/Renderer.h>
#include <siho/MeshAssimp.h>
#include <siho/Scene.h>
#include <siho/Camera.h>


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//#include <imgui.h>
//#include <backends/imgui_impl_glfw.h>

using namespace siho;

GLFWwindow* InitWindow(int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Siho", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	return window;
}

int main( void )
{
	int width = 1920, height = 1080;
	GLFWwindow* window = InitWindow(width, height);
	if (!window)
		return -1;

	/*IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();*/

	
	MeshAssimp mesh_assimp;
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
	model = glm::scale(model, glm::vec3(100.0f));
	shader.setMat4("model", model);

	Camera camera;
	camera.SetProjection(static_cast<float>(width) / height, 0.1f, 100.0f);
	UniformObject uniform_camera;
	camera.SetUniforms(uniform_camera);
	uniform_camera.apply(shader);

	glEnable(GL_DEPTH_TEST);
	do
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//ImGui_ImplGlfw_NewFrame();

		scene.render(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	//ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();
	glfwTerminate();

	return 0;

}

