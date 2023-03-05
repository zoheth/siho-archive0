#include <glad/glad.h>
#include "Window.h"

#include <siho/Camera.h>

#include <iostream>

Window::Window(int width, int height, const std::string& title)
	: width_(width), height_(height), mouse_captured_(false),
	  last_x_(static_cast<double>(width_) / 2.0), last_y_(static_cast<double>(height_) / 2.0)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
		self->CursorPosCallback(x_pos, y_pos);
	});

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(glfw_window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::MakeContextCurrent()
{
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

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::SetCamera(Camera* camera)
{
	camera_ = camera;
}

void Window::ProcessInput()
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
