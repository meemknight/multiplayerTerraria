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

struct GameData
{


}gameData;

gl2d::Renderer2D renderer;

TileRenderer tileRenderer;


bool initGame()
{
	gl2d::init();

	renderer.create();

	tileRenderer.loadAll();

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
			auto &s = tileRenderer.sprites[Tiles::grass];

			renderer.renderRectangle({100*i, 100*j, 100, 100}, {}, {},
				s.t, s.getTextureCoords(i, j));
		}



	renderer.flush();

	return true;
#pragma endregion

}

void closeGame()
{

	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
