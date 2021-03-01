#define GLM_SWIZZLE
#define PI 3.1415926535

#include "WindowManager.h"
#include "EventHandler.h"
#include "Clock.h"
#include "Camera.h"
#include "Transform.h"
#include "Mesh.h."
#include "Shader.h"
#include "Texture.h"
#include "Segment.h"
#include "FabrikSolver.h"

int main(int argc, char* args[]) {
	WindowManager wm("SDL OpenGL Application", 1600, 900);
	EventHandler input;
	Clock clock;
	Camera camera(glm::vec3(0.0f, 2.0f, 5.0f));
	//input.addMouseBinding(std::bind(&Camera::Look, &camera, std::placeholders::_1, std::placeholders::_2));
	input.addKeyBinding(SDL_SCANCODE_W, std::bind(&Camera::MoveForwards, &camera));
	input.addKeyBinding(SDL_SCANCODE_S, std::bind(&Camera::MoveBackwards, &camera));
	input.addKeyBinding(SDL_SCANCODE_D, std::bind(&Camera::MoveRight, &camera));
	input.addKeyBinding(SDL_SCANCODE_A, std::bind(&Camera::MoveLeft, &camera));

	Mesh segmentMesh("assets/segment.obj");

	Mesh targetMesh("assets/sphere.obj");
	Transform targetTransform(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.1f));

	Shader shader("src/shaders/blinn-phong");

	std::vector<Segment> chain;
	chain.push_back(Segment(glm::vec3(0.0f)));
	chain.push_back(Segment(glm::vec3(0.0f, 1.0f, 0.0f)));
	chain.push_back(Segment(glm::vec3(0.0f, 2.0f, 0.0f)));
	chain.push_back(Segment(glm::vec3(0.0f, 3.0f, 0.0f)));
	chain.push_back(Segment(glm::vec3(0.0f, 3.5f, 0.0f)));
	chain.push_back(Segment(glm::vec3(0.0f, 4.0f, 0.0f)));
	chain.push_back(Segment(glm::vec3(0.0f, 4.5f, 0.0f)));

	FabrikSolver solver(chain);

	glLineWidth(10.0f);

	while (wm.IsRunning()) {
		input.handleKeyboardInput();
		input.handleMouseInput();

		wm.Clear(0.5f, 0.5f, 0.5f, 1.0f);

		glm::vec3 P = targetTransform.GetPosition();
		ImGui::Begin("Settings");
		ImGui::SliderFloat3("Target", (float*)&P, -5.0f, 5.0f);
		ImGui::End();
		targetTransform.SetPosition(P);

		// render target
		shader.Bind();
		shader.UpdateUniform("u_ModelMatrix", targetTransform.GetModelMatrix());
		shader.UpdateUniform("u_ViewMatrix", camera.GetViewMatrix());
		shader.UpdateUniform("u_ProjectionMatrix", camera.GetProjectionMatrix());
		shader.UpdateUniform("u_BaseColour", glm::vec3(0.0f, 1.0f, 0.0f));
		shader.UpdateUniform("u_LightColour", glm::vec3(1.0f));
		shader.UpdateUniform("u_LightPosition", glm::vec3(-1.0f, 2.0f, 1.0f));
		shader.UpdateUniform("u_CameraPosition", camera.GetPosition());
		shader.UpdateUniform("u_AmbientStrength", 0.25f);
		shader.UpdateUniform("u_Shininess", 1.0f);
		targetMesh.Render();

		// solve for new target position
		std::vector<Segment> solvedChain = solver.Solve(targetTransform.GetPosition());

		// generate mesh for solved chain
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;
		for (size_t i = 0; i < solvedChain.size(); i++) {
			positions.push_back(solvedChain[i].GetPosition());
			uvs.push_back(glm::vec2(0.0));
			normals.push_back(glm::vec3(1.0f));
			indices.push_back(i);
		}
		Mesh lines(positions, uvs, normals, indices);

		// render chain
		for (size_t i = 0; i < chain.size(); i++) {
			Transform t(solvedChain[i].GetPosition(), glm::vec3(0.0f), glm::vec3(0.1f));
			shader.UpdateUniform("u_ModelMatrix", t.GetModelMatrix());
			shader.UpdateUniform("u_BaseColour", glm::vec3(0.0f, 0.0f, 1.0f));
			targetMesh.Render();
			if (i == 0) {
				Transform t(solvedChain[i].GetPosition(), glm::vec3(0.0f), glm::vec3(1.0f));
				shader.UpdateUniform("u_ModelMatrix", t.GetModelMatrix());
				shader.UpdateUniform("u_BaseColour", glm::vec3(1.0f, 0.0f, 0.0f));
				lines.Render(GL_LINE_STRIP);
			}
		}

		wm.Update();
		clock.Tick();
	}

	return 0;
}