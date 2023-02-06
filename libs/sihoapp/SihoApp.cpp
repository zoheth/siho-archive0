#include "SihoApp.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void SihoApp::run(const Config& config, SetupCallback setup, CleanupCallback cleanup, ImGuiCallback imgui, PreRenderCallback preRender, PostRenderCallback postRender, size_t width, size_t height)
{

	std::unique_ptr<SihoApp::Window> window(new SihoApp::Window(this, config, "Siho", width, height));

	scene_ = engine_->createScene();

	while(!closed_)
	{
		engine_->flush();
		engine_->execute();
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

	mMainCamera->setLensProjection(mFilamentApp->mCameraFocalLength, double(mainWidth) / height, near, far);

}

