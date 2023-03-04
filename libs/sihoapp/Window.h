#pragma once
#include <functional>
#include <string>
#include <GLFW/glfw3.h>

#include "imgui.h"


struct ImFont;
class Camera;

class Window {
public:
    Window(int width, int height, const std::string& title);

    void MakeContextCurrent();
    [[nodiscard]] bool ShouldClose() const;
    void SwapBuffers() const;
	void PollEvents();

    void SetCamera(Camera* camera);

    void drawUi(std::function<void()> uiFunc);

    void setKeyCallback(GLFWkeyfun callback);
    void setMouseButtonCallback(GLFWmousebuttonfun callback);
    void setCursorPosCallback(GLFWcursorposfun callback);
    void setScrollCallback(GLFWscrollfun callback);

private:
    GLFWwindow* glfw_window_;
    ImFontAtlas font_atlas_;
    ImFont* font_;
    Camera* camera_;

    int width_;
    int height_;

    bool mouse_captured_;
    double last_x_;
    double last_y_;
    double delta_time_{};
    double last_frame_{};

    void ProcessInput();

    void initializeImGui();
    void initializeCallbacks();
    void ErrorCallback(GLFWwindow* window, int error, const char* description);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void CursorPosCallback(double x_pos, double y_pos);
    void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset);
};
