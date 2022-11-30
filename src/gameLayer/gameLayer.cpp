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

struct GameData
{


}gameData;

gl2d::Renderer2D renderer;

Sprite dirt;


bool initGame()
{
	gl2d::init();

	renderer.create();

	dirt.load(RESOURCES_PATH "Tiles_0.png", 16, 16);

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


	for (int j = 0; j < 5; j++)
		for (int i = 0; i < 5; i++)
		{
			renderer.renderRectangle({100*i, 100*j, 100, 100}, {}, {},
				dirt.t, dirt.getTextureCoords(i, j));
		}



	renderer.flush();

	return true;
#pragma endregion

}

void closeGame()
{

	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
