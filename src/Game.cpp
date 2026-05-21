#include <Game.hpp>
#include <Shader.hpp>
#include <Texture.hpp>

#include <SDL3/SDL_timer.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <glad/gl.h>

Game::Game() :
	window("OpenGL", 1024, 600),
	model("../../res/models/DamagedHelmet/DamagedHelmet.gltf"),
	mesh(Mesh::plane(15, 15)),
	shader("../../res/shaders/shader.vert", "../../res/shaders/shader.frag"),
	shadow("../../res/shaders/shadow.vert", "../../res/shaders/shadow.frag"),
	skybox("../../res/textures/pillars_4k.hdr"),
	sun(Light::Type::Directional),
	target(0.0f)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glClearColor(0.0f, 0.4f, 0.2f, 1.0f);
}

void Game::OnAttach()
{
	ImGui::CreateContext();
	ImGui_ImplSDL3_InitForOpenGL(window.GetWindow(), window.GetContext());
	ImGui_ImplOpenGL3_Init();

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_IsSRGB;

	mesh->transform.position.y = -2.0f;
	mesh->material->tDiffuse = Texture("../../res/textures/dry_riverbed_rock_diff_2k.jpg");
	mesh->material->tRoughness = Texture("../../res/textures/dry_riverbed_rock_arm_2k.jpg");
	mesh->material->tAO = Texture("../../res/textures/dry_riverbed_rock_arm_2k.jpg");
	mesh->material->tMetalness = Texture("../../res/textures/dry_riverbed_rock_arm_2k.jpg");
	mesh->material->tNormals = Texture("../../res/textures/dry_riverbed_rock_nor_gl_2k.jpg");

	sun.transform.rotation.x = -1.0f;
}

void Game::OnDetach()
{
	ImGui_ImplSDL3_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	window.Close();
}

bool Game::OnUpdate()
{
	static float lastTime = SDL_GetTicks() / 1000.0;
	static float deltaTime;

	deltaTime = (SDL_GetTicks() / 1000.0) - lastTime;
	lastTime = SDL_GetTicks() / 1000.0;

	static float speed = 1.0f;
	static unsigned int indexed = 0;

	window.PollEvents((void(*)(SDL_Event*))ImGui_ImplSDL3_ProcessEvent);

	if (window.IsMouseGrabed())
	{
		target.x -= glm::radians(Input::motion.xrel) * 0.5f;
		target.y -= glm::radians(Input::motion.yrel) * 0.5f;
		target.y  = glm::clamp(target.y, glm::radians(-89.0f), glm::radians(89.0f));
	}

	if (Input::GetKey(SDL_SCANCODE_W))
		camera.transform.position += camera.transform.GetBasis() * glm::vec3( 0.0f,  0.0f, -1.0f) * (float)deltaTime * 8.0f * speed;
	if (Input::GetKey(SDL_SCANCODE_A))
		camera.transform.position += camera.transform.GetBasis() * glm::vec3(-1.0f,  0.0f,  0.0f) * (float)deltaTime * 8.0f * speed;
	if (Input::GetKey(SDL_SCANCODE_S))
		camera.transform.position += camera.transform.GetBasis() * glm::vec3( 0.0f,  0.0f,  1.0f) * (float)deltaTime * 8.0f * speed;
	if (Input::GetKey(SDL_SCANCODE_D))
		camera.transform.position += camera.transform.GetBasis() * glm::vec3( 1.0f,  0.0f,  0.0f) * (float)deltaTime * 8.0f * speed;
	if (Input::GetKey(SDL_SCANCODE_E))
		camera.transform.position += camera.transform.GetBasis() * glm::vec3( 0.0f,  1.0f,  0.0f) * (float)deltaTime * 8.0f * speed;
	if (Input::GetKey(SDL_SCANCODE_Q))
		camera.transform.position += camera.transform.GetBasis() * glm::vec3( 0.0f, -1.0f,  0.0f) * (float)deltaTime * 8.0f * speed;

	static glm::vec2 mp(0.0f);

	if (Input::GetKeyUp(SDL_SCANCODE_ESCAPE))
	{
		if (window.IsMouseGrabed())
		{
			window.GrabMouse(false);
			SDL_WarpMouseGlobal(mp.x, mp.y);
		}
		else
		{
			SDL_GetGlobalMouseState(&mp.x, &mp.y);
			window.GrabMouse(true);
		}
	}

	camera.transform.rotation.y += (target.x - camera.transform.rotation.y) * deltaTime * 15.0f;
	camera.transform.rotation.x += (target.y - camera.transform.rotation.x) * deltaTime * 15.0f;

	ImGui_ImplSDL3_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Light");
	ImGui::DragFloat3("position", &sun.transform.position[0], 0.01f);
	ImGui::DragFloat3("rotation", &sun.transform.rotation[0], 0.01f);
	ImGui::DragFloat("intensity", &sun.intensity, 0.01f, 0.0f, 100.0f);
	ImGui::End();

	ImGui::Begin("Model");
	ImGui::DragFloat3("position", &model[indexed]->transform.position[0], 0.01f);
	ImGui::DragFloat3("rotation", &model[indexed]->transform.rotation[0], 0.01f);
	ImGui::DragFloat3("scale", &model[indexed]->transform.scale[0], 0.01f);
	ImGui::End();
	ImGui::Begin("Mesh");
	ImGui::DragFloat3("position", &mesh->transform.position[0], 0.01f);
	ImGui::DragFloat3("rotation", &mesh->transform.rotation[0], 0.01f);
	ImGui::DragFloat3("scale", &mesh->transform.scale[0], 0.01f);
	ImGui::End();

	ImGui::Begin("Properties");
	ImGui::DragFloat3("position", &camera.transform.position[0], 0.01f);
	ImGui::DragFloat2("rotation", &target[0], 0.01f);
	ImGui::DragFloat("speed", &speed, 0.01f, 0.0f, 5.0f);
	ImGui::SliderFloat("intensity", &intensity, 0.0f, 1.0f);
	ImGui::SliderFloat("ibl", &ibl, 0.0f, 1.0f);
	ImGui::SliderInt("mode", &mode, 0, 7);
	ImGui::Text("%u", indexed);
	ImGui::SameLine();
	if (ImGui::Button("+"))
		indexed++;
	ImGui::SameLine();
	if (ImGui::Button("-"))
		indexed++;
	ImGui::End();

	ImGui::Begin("Shadow Map", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
	ImGui::Image(
		sun.GetShadowMap(),
		{
			ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2.0f,
			ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2.0f,
		}
	);
	ImGui::End();

	return window.ShouldClose();
}

void Game::OnRender()
{
	int w, h;
	window.GetSize(&w, &h);
	glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -20.0f, 20.0f) * sun.transform.GetLookAt();
	sun.BindShadowMap();
	shadow.Bind();
	shadow.SetMat4("uProjection", &projection[0][0]);
	model.Draw(shadow);
	mesh->Draw(shadow);
	sun.UnBind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, w, h);

	skybox.Draw(&(glm::perspective(glm::radians(75.0f), (float)w/(float)h, 0.01f, 100.0f) * glm::mat4(glm::mat3(camera.transform.GetLookAt())))[0][0]);
	skybox.SetSkyboxIntensity(intensity);
	
	shader.Bind();
	shader.SetInt("uTextureDiffuse", 0);
	shader.SetInt("uTextureAO", 1);
	shader.SetInt("uTextureNormals", 2);
	shader.SetInt("uTextureRoughness", 3);
	shader.SetInt("uTextureMetalness", 4);
	shader.SetInt("uTextureEmissive", 5);
	shader.SetInt("uTextureSkybox", 6);
	shader.SetInt("uShadowMap", 7);
	shader.SetMat4("uShadowCoords", &projection[0][0]);
	shader.SetFloat("uIntensity", ibl);
	shader.SetInt("uMode", mode);
	shader.SetVec3("uCameraPosition", &camera.transform.position[0]);
	sun.Setup(shader);
	shader.SetMat4("uCamera", &(glm::perspective(glm::radians(75.0f), (float)w/(float)h, 0.01f, 100.0f) * camera.transform.GetLookAt())[0][0]);
	shader.SetFloat("uTime", SDL_GetTicks() / 1000.0f);

	model.Draw(shader);
	mesh->Draw(shader);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	window.Swap();
}
