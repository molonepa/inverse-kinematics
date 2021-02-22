#define GLM_SWIZZLE

#include "WindowManager.h"
#include "EventHandler.h"
#include "Clock.h"
#include "Camera.h"
#include "Transform.h"
#include "Mesh.h."
#include "Shader.h"
#include "Texture.h"

int main(int argc, char* args[]) {
	WindowManager wm("SDL OpenGL Application", 1600, 900);
	EventHandler input;
	Clock clock;
	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
	input.addMouseBinding(std::bind(&Camera::Look, &camera, std::placeholders::_1, std::placeholders::_2));
	input.addKeyBinding(SDL_SCANCODE_W, std::bind(&Camera::MoveForwards, &camera));
	input.addKeyBinding(SDL_SCANCODE_S, std::bind(&Camera::MoveBackwards, &camera));
	input.addKeyBinding(SDL_SCANCODE_D, std::bind(&Camera::MoveRight, &camera));
	input.addKeyBinding(SDL_SCANCODE_A, std::bind(&Camera::MoveLeft, &camera));

	while (wm.IsRunning()) {
		input.handleKeyboardInput();
		input.handleMouseInput();

		wm.Clear(0.5f, 0.5f, 0.5f, 1.0f);
		
		ImGui::Begin("Settings");
		ImGui::End();

		wm.Update();
		clock.Tick();
	}

	return 0;
}