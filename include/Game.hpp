#pragma once

#include "App.hpp"
#include "Window.hpp"
#include "Model.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Skybox.hpp"

#include <gtc/matrix_transform.hpp>
#include <glm.hpp>

class Game : AppLayer
{
public:
	Game();

	void OnAttach();
	void OnDetach();
	bool OnUpdate();
	void OnRender();

private:
	Window window;
	Model model;
	Mesh* mesh;
	Shader shader;
	Shader shadow;
	Light sun;
	Camera camera;
	Skybox skybox;

	glm::vec2 target;
	float intensity = 1.0f;
	float ibl = 1.0f;
	int mode = 0;
};
