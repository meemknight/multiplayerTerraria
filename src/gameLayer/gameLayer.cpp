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

struct GameData
{


}gameData;

gl2d::Renderer2D renderer;

TileRenderer tileRenderer;
Map map;

glm::vec2 playerPos = {};

bool initGame()
{
	gl2d::init();

	renderer.create();
	renderer.currentCamera.zoom = 100;

	tileRenderer.loadAll();

	map.create(100, 100);

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

	
	renderer.currentCamera.follow(playerPos, 1, 0.01, 3, w, h);

	tileRenderer.renderMap(renderer, map);

	ImGui::Begin("camera");

	ImGui::DragFloat("zoom", &renderer.currentCamera.zoom, 1, 0.00001, 500);
	ImGui::DragFloat2("player pos", &playerPos[0], 0.002);



	ImGui::End();



	renderer.flush();

	return true;
#pragma endregion

}

void closeGame()
{

	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
