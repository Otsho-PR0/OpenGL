#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>

#include "Camera.hpp"
#include "Shader.hpp"
#include "Light.hpp"
#include "Model.hpp"

Camera camera({0,1,0});

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1024, 600, "OpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetCursorPosCallback(window, cursorPosCallback);

	stbi_set_flip_vertically_on_load(true);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");
	
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigErrorRecoveryEnableDebugLog = false;
	io.ConfigErrorRecoveryEnableAssert = false;

	io.Fonts->AddFontDefault();
	
    ImFontConfig config;
    config.MergeMode = true;
	config.GlyphOffset.y = 2;
	config.GlyphMinAdvanceX = 13;

	static const ImWchar icon_ranges[] =
	{
		0x0020, 0x00FF,
		0xE000, 0xF8FF,
		0,
	};
	ImFont* nerdFont = io.Fonts->AddFontFromFileTTF("../../res/fonts/JetBrainsMonoNerdFont-Regular.ttf", 22, &config, icon_ranges);
	
    io.Fonts->Build();
	ImGui_ImplOpenGL3_CreateFontsTexture();

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), 1024.0f / 600.0f, 0.01f, 100.0f);
	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

	Shader shader("../../res/shaders/shader.vert", "../../res/shaders/shader.frag");
	Model instance("../""../res/models/scene.gltf");

	shader.use();
	shader.setMat4("uProjection", &projection[0][0]);
	shader.setMat4("uView", &camera.getViewMatrix()[0][0]);
	shader.setMat4("uModel", &model[0][0]);
	shader.setInt("uMaterialDiffuse", 0);
	shader.setInt("uMaterialRoughness", 1);
	shader.setInt("uMaterialNormals", 2);

	float lastTime = glfwGetTime();
	float deltaTime;

	bool simulate = false;

	Light sun(Light::Type::Directional);
	sun.ambient   = {.5,.5,.5};
	sun.diffuse   = {.2,.2,.2};
	sun.specular  = {.3,.3,.3};
	sun.direction = {0,-1,0};
	sun.shininess = 32;
	shader.setVec3("light.direction", &sun.direction[0]);
	shader.setVec3("light.ambient", &sun.ambient[0]);
	shader.setVec3("light.diffuse", &sun.diffuse[0]);
	shader.setVec3("light.specular", &sun.specular[0]);
	shader.setFloat("light.shininess", sun.shininess);


	float speed = 0.01f;

	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		if (glfwGetKey(window, GLFW_KEY_E))
			camera.position.y += speed;
		if (glfwGetKey(window, GLFW_KEY_Q))
			camera.position.y -= speed;
		if (glfwGetKey(window, GLFW_KEY_W))
			camera.position += glm::normalize(glm::vec3(camera.direction.x, 0, camera.direction.z)) * speed;
		if (glfwGetKey(window, GLFW_KEY_S))
			camera.position -= glm::normalize(glm::vec3(camera.direction.x, 0, camera.direction.z)) * speed;
		if (glfwGetKey(window, GLFW_KEY_D))
			camera.position += glm::cross(glm::normalize(glm::vec3(camera.direction.x, 0, camera.direction.z)), glm::vec3(0,1,0)) * speed;
		if (glfwGetKey(window, GLFW_KEY_A))
			camera.position -= glm::cross(glm::normalize(glm::vec3(camera.direction.x, 0, camera.direction.z)), glm::vec3(0,1,0)) * speed;

		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		shader.setInt("doIt", simulate);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.setMat4("uView", &camera.getViewMatrix()[0][0]);
		shader.setVec3("uCameraPosition", &camera.position[0]);
		shader.setMat4("uModel", &model[0][0]);
		instance.draw();

		ImGui::Begin("Toolbar", NULL,
             ImGuiWindowFlags_NoResize |
             ImGuiWindowFlags_NoMove   |
			 ImGuiWindowFlags_NoScrollbar |
             ImGuiWindowFlags_NoCollapse |
             ImGuiWindowFlags_NoTitleBar |
			 ImGuiWindowFlags_NoDocking
		);
		ImGui::SetWindowSize({100,30});
		ImGui::SetWindowPos({ImGui::GetMainViewport()->Size.x / 2 - ImGui::GetWindowWidth() / 2, 2});
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2) - 27);
		ImGui::SetCursorPosY((ImGui::GetWindowHeight() / 2) - 13);
		ImVec2 icon_size = ImGui::CalcTextSize("\uead1");
		if (ImGui::Button(simulate ? "\uead1" : "\uead3", ImVec2(26,26)))
			simulate = !simulate;
		ImGui::SameLine(0,2);
		if (ImGui::Button("\uead7", ImVec2(26,26)))
			simulate = false;
		ImGui::End();

		ImGui::Begin("Properties");
		if (ImGui::DragFloat3("direction", &sun.direction[0], 0.01f, -1, 1))
		{
			sun.direction = glm::normalize(sun.direction);
			shader.setVec3("light.direction", &sun.direction[0]);
		}
		ImGui::DragFloat("speed", &speed);
		ImGui::Text("DoIT: %i", simulate);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive())
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && !glfwGetKey(window, GLFW_KEY_LEFT_ALT))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	static glm::vec2 oldCursorPos = {xPos, yPos};
	static float yaw   = -90;
	static float pitch =  0;

	if (glm::length(glm::vec2(xPos, yPos) - oldCursorPos) > 50.0f)
	{
		oldCursorPos = {xPos, yPos};
		return;
	}

	yaw += xPos - oldCursorPos.x;
	pitch += yPos - oldCursorPos.y;

	pitch = glm::clamp(pitch, -89.0f, 89.0f);

	camera.direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	camera.direction.y = -glm::sin(glm::radians(pitch));
	camera.direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

	oldCursorPos = {xPos, yPos};
}
