#pragma once

#ifdef _WIN32 || _WIN64
	#include <windows.h>
#endif

#include <string>
#include <stdexcept>

#include <SDL.h>
#include <GL/glew.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

class WindowManager {
public:
	WindowManager(const std::string& title, int width, int height);
	virtual ~WindowManager();

	void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void Update();

	inline bool IsRunning() { return m_IsRunning; }

private:
	SDL_Window* mp_Window;
	SDL_GLContext m_GLContext;
	bool m_IsRunning;
};

