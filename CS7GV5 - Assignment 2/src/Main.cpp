#define GLM_SWIZZLE
#define PI 3.1415926535

#include <iostream>
#include <string>

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
	Camera camera(glm::vec3(0.0f, 2.0f, 10.0f));
	//input.addMouseBinding(std::bind(&Camera::Look, &camera, std::placeholders::_1, std::placeholders::_2));
	input.addKeyBinding(SDL_SCANCODE_W, std::bind(&Camera::MoveForwards, &camera));
	input.addKeyBinding(SDL_SCANCODE_S, std::bind(&Camera::MoveBackwards, &camera));
	input.addKeyBinding(SDL_SCANCODE_D, std::bind(&Camera::MoveRight, &camera));
	input.addKeyBinding(SDL_SCANCODE_A, std::bind(&Camera::MoveLeft, &camera));

	Mesh plane("assets/plane.obj");
	Mesh sphere("assets/sphere.obj");

	Texture gridTexture("assets/test_texture.jpg");

	Transform targetTransform(glm::vec3(0.0f, 5.5f, 0.0f), glm::vec3(0.0f), glm::vec3(0.1f));
	Transform groundTransform;

	Shader shader("src/shaders/blinn-phong");
	Shader textureShader("src/shaders/blinn-phong-textured");

	std::vector<Segment> chain;
	std::vector<float> lengths;
	for (int i = 0; i < 10; i++) {
		chain.push_back(Segment(glm::vec3(0.0f, (float)0.25f*i, 0.0f)));
	}
	FabrikSolver solver(chain);

	glLineWidth(4.0f);

	while (wm.IsRunning()) {
		input.handleKeyboardInput();
		input.handleMouseInput();

		wm.Clear(0.5f, 0.5f, 0.5f, 1.0f);

		// render ground
		textureShader.Bind();
		textureShader.UpdateUniform("u_ModelMatrix", groundTransform.GetModelMatrix());
		textureShader.UpdateUniform("u_ViewMatrix", camera.GetViewMatrix());
		textureShader.UpdateUniform("u_ProjectionMatrix", camera.GetProjectionMatrix());
		textureShader.UpdateUniform("u_TextureScale", 10.0f);
		textureShader.UpdateUniform("u_Albedo", 0);
		gridTexture.Bind(0);
		textureShader.UpdateUniform("u_LightColour", glm::vec3(1.0f));
		textureShader.UpdateUniform("u_LightPosition", glm::vec3(-1.0f, 2.0f, 1.0f));
		textureShader.UpdateUniform("u_CameraPosition", camera.GetPosition());
		textureShader.UpdateUniform("u_AmbientStrength", 0.75f);
		textureShader.UpdateUniform("u_Shininess", 1.0f);
		plane.Render();

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
		sphere.Render();

		// solve for new target position
		chain = solver.Solve(targetTransform.GetPosition());
		lengths = solver.GetLengths();

		// generate mesh for solved chain
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;
		for (size_t i = 0; i < chain.size(); i++) {
			positions.push_back(chain[i].GetPosition());
			uvs.push_back(glm::vec2(0.0));
			normals.push_back(glm::vec3(1.0f));
			indices.push_back(i);
		}
		Mesh lines(positions, uvs, normals, indices);

		// render chain
		for (size_t i = 0; i < chain.size(); i++) {
			Transform t(chain[i].GetPosition(), glm::vec3(0.0f), glm::vec3(0.1f));
			shader.UpdateUniform("u_ModelMatrix", t.GetModelMatrix());
			shader.UpdateUniform("u_BaseColour", glm::vec3(0.0f, 0.0f, 1.0f));
			sphere.Render();
			if (i == 0) {
				Transform t(chain[i].GetPosition(), glm::vec3(0.0f), glm::vec3(1.0f));
				shader.UpdateUniform("u_ModelMatrix", t.GetModelMatrix());
				shader.UpdateUniform("u_BaseColour", glm::vec3(1.0f, 0.0f, 0.0f));
				lines.Render(GL_LINE_STRIP);
			}
		}

		glm::vec3 P = targetTransform.GetPosition();

		ImGui::Begin("Settings");
			ImGui::DragFloat3("Target position (X Y Z)", (float*)&P, 0.01f, -50.0f, 50.0f);
			//if (ImGui::Button("Add segment")) {
			//	glm::vec3 p = chain.back().GetPosition();
			//	chain.push_back(Segment(p));
			//	lengths.push_back(0.0f);
			//}
			//ImGui::SameLine();
			//if (ImGui::Button("Remove segment")) {
			//	chain.pop_back();
			//	lengths.pop_back();
			//}
			for (size_t i = 0; i < lengths.size(); i++) {
				std::string label = "Segment " + std::to_string(i + 1) + " length";
				float l = lengths[i];
				ImGui::SliderFloat(label.c_str(), &l, 0.1f, 10.0f);
				lengths[i] = l;
			}
		ImGui::End();

		if (P.y < 0) {
			P.y = 0;
		}
		targetTransform.SetPosition(P);

		// update with new lengths
		solver.SetLengths(lengths);

		wm.Update();
		clock.Tick();
	}

	return 0;
}