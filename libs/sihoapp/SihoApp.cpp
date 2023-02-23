#include "SihoApp.h"

#include <glad/glad.h>
#include <iostream>
#include <siho/Renderer.h>
#include <siho/Engine.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/EntityManager.h"

using namespace siho;

void SihoApp::run(const Config& config, const SetupCallback& setup, CleanupCallback cleanup, ImGuiCallback imgui, const PreRenderCallback
                  & pre_render, const PostRenderCallback& post_render, const size_t width, const size_t height)
{

	std::unique_ptr<SihoApp::Window> window(new SihoApp::Window(this, config, "Siho", width, height));

	siho::Renderer* renderer = window->getRenderer();

	mScene = mEngine->createScene();

	for (auto& view : window->mViews) {
		if (view.get() != window->mUiView) {
			view->getView()->setScene(mScene);
		}
	}
	setup(mEngine, window->mMainView->getView(), mScene);

	while(!mClosed)
	{
		mEngine->execute();
		if (pre_render) {
			pre_render(mEngine, window->mViews[0]->getView(), mScene, renderer);
		}
		for (const siho::View* offscreen_view : mOffscreenViews) {
			renderer->render(offscreen_view);
		}
		for (auto const& view : window->mViews) {
			renderer->render(view->getView());
		}
		if (post_render) {
			post_render(mEngine, window->mViews[0]->getView(), mScene, renderer);
		}
		//renderer->endFrame();
	}



}

SihoApp::CView::CView(siho::Renderer& renderer, std::string name)
	: mEngine(*renderer.getEngine()), mName(std::move(name))
{
	mView = mEngine.createView();
	mView->setName(name);
}

SihoApp::Window::Window(SihoApp* app, const Config& config, const std::string& title, int w, int h)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
	if (mWindow == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	mApp->mEngine = Engine::create();
	mRenderer = mApp->mEngine->createRenderer();

	utils::EntityManager::get().create(1, &mCameraEntity);
	mCamera = mApp->mEngine->createCamera(mCameraEntity);
	mViews.emplace_back(mMainView = new CView(*mRenderer, "Main View"));
	mMainView->setCamera(mCamera);

	configureCamerasForWindow();
}

SihoApp::Window::~Window()
{
}

void SihoApp::Window::configureCamerasForWindow()
{
	const uint32_t width = mWidth;
	const uint32_t height = mHeight;

	const glm::vec3 at(0., 0., -4.);
	const double ratio = static_cast<double>(height) / static_cast<double>(width);
	constexpr float near = 0.1f;
	constexpr float far = 100.f;
	constexpr float fov = 45.f;

	mCamera->setProjection(fov, static_cast<double>(width) / height, near, far);

}

