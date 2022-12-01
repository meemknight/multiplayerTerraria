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

#undef min
#undef max

struct GameData
{
	float zoom = 1;
	glm::vec2 playerPos = {};


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

	platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

	renderer.currentCamera.zoom = gameData.zoom;


	return true;
}



bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w= platform::getWindowSizeX();
	h = platform::getWindowSizeY();
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1, 1, 1, 0);

	renderer.updateWindowMetrics(w, h);

#pragma endregion

	
	renderer.currentCamera.follow(gameData.playerPos, 1, 0.01, 3, w, h);

	tileRenderer.renderMap(renderer, map);

	playerRenderer.render(renderer, gameData.playerPos);


#pragma region imgui
	ImGui::Begin("camera");

	ImGui::DragFloat("zoom", &renderer.currentCamera.zoom, 1, 1, 500);
	ImGui::DragFloat2("player pos", &gameData.playerPos[0], 5);
	
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
