#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include "sprites.h"
#include <tileRenderer.h>
#include "map.h"
#include "playerRenderer.h"
#include <entity.h>

#undef min
#undef max

struct GameData
{
	float zoom = 1;
	Player player;

}gameData;

gl2d::Renderer2D renderer;

TileRenderer tileRenderer;
Map map;

PlayerRenderer playerRenderer;


bool initGame()
{
	gl2d::init();

	renderer.create();
	renderer.currentCamera.zoom = 100;

	tileRenderer.loadAll();

	playerRenderer.loadAll();

	generateMap(map, 1234);

	map.renderMapIntoTexture();

	if (!platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData)))
	{
		gameData = GameData();
	}

	renderer.currentCamera.zoom = gameData.zoom;

	gameData.player.position.size = PLAYER_SIZE;

	return true;
}



bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getWindowSizeX();
	h = platform::getWindowSizeY();

	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1, 1, 1, 0);

	renderer.updateWindowMetrics(w, h);

#pragma endregion

#pragma region input
	
	if(0)
	{
		glm::vec2 direction = {};

		if (platform::isKeyHeld(platform::Button::A))
		{
			direction.x -= 1;
		}

		if (platform::isKeyHeld(platform::Button::D))
		{
			direction.x += 1;
		}

		if (platform::isKeyHeld(platform::Button::W))
		{
			direction.y -= 1;
		}

		if (platform::isKeyHeld(platform::Button::S))
		{
			direction.y += 1;
		}

		if (length(direction) != 0.f)
		{
			direction = normalize(direction);
		}

		gameData.player.position.position += deltaTime * direction * 20.f;
	}
	else
	{
		float direction = {};

		if (platform::isKeyHeld(platform::Button::A))
		{
			direction -= 1;
		}

		if (platform::isKeyHeld(platform::Button::D))
		{
			direction += 1;
		}

		gameData.player.moveVelocityX(10 * deltaTime * direction);

		if (platform::isKeyPressedOn(platform::Button::Space))
		{
			gameData.player.jump();
		}
	}



#pragma endregion

#pragma region player phisics

	gameData.player.applyGravity(9.f);

	gameData.player.updatePhisics(deltaTime);

	gameData.player.resolveConstrains(map);

	gameData.player.updateMove();

#pragma endregion

	
	renderer.currentCamera.follow(gameData.player.position.getCenter(), 0.5, 0.001, 3, w, h);

	tileRenderer.renderMap(renderer, map);

	playerRenderer.render(renderer, gameData.player.position.position, gameData.player.skin,
		gameData.player.movingRight);


#pragma region imgui
	ImGui::Begin("camera");

	ImGui::DragFloat("zoom", &renderer.currentCamera.zoom, 1, 1, 500);
	ImGui::DragFloat2("player pos", &gameData.player.position.position[0], 5);
	
	gameData.zoom = renderer.currentCamera.zoom;

	{
		auto s = ImGui::GetContentRegionMax();

		if (ImGui::BeginChild(6996, {}, false, ImGuiWindowFlags_HorizontalScrollbar))
		{

			float xsize = std::max((int)(s.x * 1) - 10, (int)(100 * 1));
			float aspect = (float)map.mapSize.x/map.mapSize.y;

			ImGui::Image((void *)map.texture, {2400,2400 / aspect},
				{0, 0}, {1, 1});

			ImGui::EndChild();
		}
	}


	ImGui::End();
#pragma endregion



	renderer.flush();

	return true;
#pragma endregion

}

void closeGame()
{

	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
