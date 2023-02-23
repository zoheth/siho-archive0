#pragma once
#include <GLFW/glfw3.h>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <siho/Camera.h>
#include <siho/Config.h>
#include <siho/Engine.h>
#include <siho/View.h>
#include <siho/Scene.h>
#include <siho/Renderer.h>

class SihoApp
{
public:
	using SetupCallback = std::function<void(siho::Engine*, siho::View*, siho::Scene*)>;
	using CleanupCallback = std::function<void(siho::Engine*, siho::View*, siho::Scene*)>;
	using PreRenderCallback = std::function<void(siho::Engine*, siho::View*, siho::Scene*, siho::Renderer*)>;
	using PostRenderCallback = std::function<void(siho::Engine*, siho::View*, siho::Scene*, siho::Renderer*)>;
	using ImGuiCallback = std::function<void(siho::Engine*, siho::View*)>;
	using ResizeCallback = std::function<void(siho::Engine*, siho::View*)>;

	static SihoApp& get();

	void run(const Config& config, const SetupCallback& setup, CleanupCallback cleanup,
		ImGuiCallback imgui = ImGuiCallback(), const PreRenderCallback& pre_render = PreRenderCallback(),
		const PostRenderCallback& post_render = PostRenderCallback(),
		size_t width = 1024, size_t height = 640);
private:
	class CView
	{
	public:
		CView(siho::Renderer& renderer, std::string name);

		siho::View* getView() const { return mView; }

		void setCamera(siho::Camera* camera) const { mView->setCullingCamera(camera); }
	private:
		siho::Engine& mEngine;
		siho::View* mView = nullptr;
		std::string mName;
	};

	/**
	 * Window is a wrapper around a glfw window, a renderer, a main view and a ui view.
	 */
	class Window
	{
		friend class SihoApp;
	public:
		Window(SihoApp* app, const Config& config, const std::string& title, int w, int h);
		virtual ~Window();

		siho::Renderer* getRenderer() const { return mRenderer; }

	private:
		void configureCamerasForWindow();
		SihoApp* const mApp = nullptr;

		GLFWwindow* mWindow;

		siho::Renderer* mRenderer;

		utils::Entity mCameraEntity;
		siho::Camera* mCamera;

		std::vector<std::unique_ptr<CView>> mViews;
		CView* mMainView;
		CView* mUiView;

		size_t mWidth = 0;
		size_t mHeight = 0;
		size_t mLastX = 0;
		size_t mLastY = 0;
	};

	friend  class Window;

	siho::Engine* mEngine = nullptr;
	siho::Scene* mScene = nullptr;
	bool mClosed = false;
	uint64_t mTime = 0;

	std::string mWindowTitle;
	std::vector<siho::View*> mOffscreenViews;

	float mCameraFocalLength = 28.0f;
};
