#include <glad/glad.h>
#include "Window.h"

#include <siho/Camera.h>

#include <iostream>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

Window::Window(int width, int height, const std::string& title)
	: width_(width), height_(height), mouse_captured_(false),
	  last_x_(static_cast<double>(width_) / 2.0), last_y_(static_cast<double>(height_) / 2.0)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glfw_window_ = glfwCreateWindow(width, height, "Siho", nullptr, nullptr);
	if (glfw_window_ == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(glfw_window_);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glfwSetWindowUserPointer(glfw_window_, this);

	glfwSetCursorPosCallback(glfw_window_, +[](GLFWwindow* window, const double x_pos, const double y_pos)
	{
		const auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(self->roam_)
			self->CursorPosCallback(x_pos, y_pos);
	});

	glEnable(GL_DEPTH_TEST);

	InitUi();
}

void Window::ProcessContext()
{
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	const double current_frame = glfwGetTime();
	delta_time_ = current_frame - last_frame_;
	last_frame_ = current_frame;
	ProcessInput();
}

bool Window::ShouldClose() const
{
	return glfwGetKey(glfw_window_, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(glfw_window_) == 0;
}

void Window::SwapBuffers() const
{
	glfwSwapBuffers(glfw_window_);
}

void Window::DrawUi() const
{
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(glfw_window_, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::SetCamera(Camera* camera)
{
	camera_ = camera;
}

void Window::ShowLightEditorWindow(Scene& scene)
{
	ImGui::Begin("Light Editor", &show_light_editor_);
	if (ImGui::Button("Add Light")) {
		scene.AddLight(PointLight{
			glm::vec3(0.0f, 10.0f, 10.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			250.0f });
	}
	/*ImGui::SameLine();
        if (selectedLight >= 0 && selectedLight < m_lights.size()) {
            if (ImGui::Button("Remove Light")) {
                removeLight(selectedLight);
                selectedLight = -1;
            }
        }*/

	for (int i = 0; i < scene.point_light_count(); i++) {
		ImGui::PushID(i);
		if (ImGui::CollapsingHeader(("Light " + std::to_string(i)).c_str())) {
			PointLight& light = scene.point_light(i);

			ImGui::SliderFloat3("Position", reinterpret_cast<float*>(&light.position), -10.0f, 10.0f);
			ImGui::ColorEdit3("Color", reinterpret_cast<float*>(&light.color));
			ImGui::SliderAngle("Rotation", &light.angle_y, 0.0f, 360.0f);
			ImGui::DragFloat("Intensity", &light.intensity);
		}
		ImGui::PopID();
	}

	ImGui::End();
}

void Window::InitUi() const
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// imgui_io = ImGui::GetIO(); (void)imgui_io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(glfw_window_, true);
	ImGui_ImplOpenGL3_Init();
}

void Window::ProcessInput()
{
	if (glfwGetMouseButton(glfw_window_, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
		roam_ = true;
		glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		roam_ = false;
		mouse_captured_ = false;
		glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if(roam_)
	{
		if (glfwGetKey(glfw_window_, GLFW_KEY_W) == GLFW_PRESS)
			camera_->ProcessKeyboard(FORWARD, delta_time_);

		if (glfwGetKey(glfw_window_, GLFW_KEY_S) == GLFW_PRESS)
			camera_->ProcessKeyboard(BACKWARD, delta_time_);

		if (glfwGetKey(glfw_window_, GLFW_KEY_A) == GLFW_PRESS)
			camera_->ProcessKeyboard(LEFT, delta_time_);

		if (glfwGetKey(glfw_window_, GLFW_KEY_D) == GLFW_PRESS)
			camera_->ProcessKeyboard(RIGHT, delta_time_);
	}
	
}

void Window::CursorPosCallback(const double x_pos, const double y_pos)
{
	if (!camera_)
		return;
	if (!mouse_captured_)
	{
		last_x_ = x_pos;
		last_y_ = y_pos;
		mouse_captured_ = true;
	}
	const double x_offset = x_pos - last_x_;
	const double y_offset = last_y_ - y_pos;
	last_x_ = x_pos;
	last_y_ = y_pos;
	camera_->ProcessMouseMovement(x_offset, y_offset);
}
