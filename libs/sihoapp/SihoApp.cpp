#include "SihoApp.h"
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

