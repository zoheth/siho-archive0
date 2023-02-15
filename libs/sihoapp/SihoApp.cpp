#include "SihoApp.h"

#include <glad/glad.h>
#include <iostream>
#include <siho/Renderer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void SihoApp::run(const Config& config, const SetupCallback& setup, CleanupCallback cleanup, ImGuiCallback imgui, const PreRenderCallback
                  & pre_render, const PostRenderCallback& post_render, const size_t width, const size_t height)
{

	std::unique_ptr<SihoApp::Window> window(new SihoApp::Window(this, config, "Siho", width, height));

	siho::Renderer* renderer = window->getRenderer();

	scene_ = engine_->createScene();

	for (auto& view : window->views_) {
		if (view.get() != window->ui_view_) {
			view->getView()->setScene(scene_);
		}
	}
	setup(engine_, window->main_view_->getView(), scene_);

	while(!closed_)
	{
		engine_->execute();
		if (pre_render) {
			pre_render(engine_, window->views_[0]->getView(), scene_, renderer);
		}
		for (const siho::View* offscreen_view : offscreen_views_) {
			renderer->render(offscreen_view);
		}
		for (auto const& view : window->views_) {
			renderer->render(view->getView());
		}
		if (post_render) {
			post_render(engine_, window->views_[0]->getView(), scene_, renderer);
		}
		//renderer->endFrame();
	}



}

SihoApp::Window::Window(SihoApp* app, const Config& config, const std::string& title, int w, int h)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
	if (window_ == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window_);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
}

void SihoApp::Window::configureCamerasForWindow()
{
	const uint32_t width = width_;
	const uint32_t height = height_;

	const glm::vec3 at(0., 0., -4.);
	const double ratio = static_cast<double>(height) / static_cast<double>(width);
	const double near = 0.1;
	const double far = 100;

	main->setLensProjection(mFilamentApp->mCameraFocalLength, double(mainWidth) / height, near, far);

}

