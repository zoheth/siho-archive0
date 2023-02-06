#pragma once

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

	void run(const Config& config, SetupCallback setup, CleanupCallback cleanup,
		ImGuiCallback imgui = ImGuiCallback(), PreRenderCallback preRender = PreRenderCallback(),
		PostRenderCallback postRender = PostRenderCallback(),
		size_t width = 1024, size_t height = 640);
private:
	class CView
	{
		
	};

	class Window
	{
		friend class SihoApp;
	public:
		Window(SihoApp* app, const Config& config, std::string title, size_t w, size_t h);
		virtual ~Window();
	private:
		void configureCamerasForWindow();

		siho::Renderer* renderer_;
		siho::Camera* camera_;

		CView* main_view_;
		CView* ui_view_;

		size_t width_ = 0;
		size_t height_ = 0;
		size_t last_x_ = 0;
		size_t last_y_ = 0;
	};

	friend  class Window;

	siho::Engine* engine_ = nullptr;
	siho::Scene* scene_ = nullptr;
	bool closed_ = false;
	uint64_t time_ = 0;

	std::string window_title_;

};
