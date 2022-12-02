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
#include <client.h>
#include <glui/glui.h>
#include <enet/enet.h>
#include <server.h>
#include <items.h>
#include <npcs.h>

#undef min
#undef max

struct GameData
{
	PlayerSkin playerSkin = {};

}gameData;

gl2d::Renderer2D renderer;

TileRenderer tileRenderer;

PlayerRenderer playerRenderer;

ItemRenderer itemRenderer;

NpcRenderer npcRenderer;

gl2d::Texture logoTexture;
gl2d::Texture splashScreenTexture;
gl2d::Texture uiTexture;

gl2d::Font font;

bool gameplayRunning = false;


bool initGame()
{
	gl2d::init();
	glui::gluiInit();
	enet_initialize();

	renderer.create();
	renderer.currentCamera.zoom = 100;

	tileRenderer.loadAll();
	playerRenderer.loadAll();
	itemRenderer.loadAll();
	npcRenderer.load();

	logoTexture.loadFromFile(RESOURCES_PATH "Logo.png");
	splashScreenTexture.loadFromFile(RESOURCES_PATH "splash.png");
	uiTexture.loadFromFile(RESOURCES_PATH "InnerPanelBackground.png", true);

	font.createFromFile(RESOURCES_PATH "roboto_black.ttf");


	if (!platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData)))
	{
		gameData = GameData();
	}

	glClearColor(0.4, 0.6, 0.9, 0);

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

	renderer.updateWindowMetrics(w, h);

#pragma endregion

	if (!gameplayRunning)
	{
		glui::Begin(1);
		{
			glui::Texture(1, logoTexture);

			if(glui::Button("Start server", Colors_White, uiTexture))
			{
				
				launchServer();

				if (joinServer(gameData.playerSkin, ""))
				{
					gameplayRunning = true;
				}
				else
				{
					std::cout << "Error joining server\n";
				}
			}

			glui::BeginMenu("Join game", Colors_White, uiTexture);
			{
				static char ip[17] = {};

				glui::Text("enter ip: ", Colors_White);
				glui::Text("Leave blank for localhost", Colors_White);
				glui::InputText("input ip", ip, sizeof(ip));

				if (glui::Button("Join", Colors_White, uiTexture))
				{
					if (joinServer(gameData.playerSkin, ip))
					{
						gameplayRunning = true;
					}
					else
					{
						std::cout << "Error joining server\n";
					}
				}
			
			}
			glui::EndMenu();

			glui::BeginMenu("Costumize character", Colors_White, uiTexture);
			{
				glui::colorPicker("Hair Color", &gameData.playerSkin.hairColor[0], uiTexture, uiTexture);
				glui::sliderInt("Hair Type", &gameData.playerSkin.hairType, 0, 19, uiTexture, {1,1,1,1}, uiTexture);
				glui::colorPicker("Eye color", &gameData.playerSkin.eyeColor[0], uiTexture, uiTexture);
				glui::colorPicker("Skin color", &gameData.playerSkin.skinColor[0], uiTexture, uiTexture);
				glui::colorPicker("Clothes color", &gameData.playerSkin.clothesColor[0], uiTexture, uiTexture);
				glui::colorPicker("Pants color", &gameData.playerSkin.pantsColor[0], uiTexture, uiTexture);
				
				glui::newColum(1);

				glm::vec4 transform = {};
				bool hovered = false;
				if (glui::CustomWidget(2, &transform, &hovered))
				{
					renderer.pushCamera();

					static PlayerAnimation animation;

					if (hovered)
					{
						animation.state = PlayerAnimation::running;
					}
					else
					{
						animation.state = PlayerAnimation::stay;
					}

					animation.update(deltaTime);

					playerRenderer.render(renderer, glm::vec2{transform.x, transform.y}, gameData.playerSkin, true, animation,
						100);

					renderer.popCamera();
				}

				glui::Toggle("Has clothes", Colors_White, &gameData.playerSkin.hasClothes, uiTexture, uiTexture);
				glui::Toggle("Has pants", Colors_White, &gameData.playerSkin.hasPants, uiTexture, uiTexture);
				

			}
			glui::EndMenu();

		}
		glui::End();

		glui::renderFrame(renderer, font, platform::getRelMousePosition(), platform::isLMousePressed(),
			platform::isLMouseHeld(), platform::isLMouseReleased(), platform::isKeyReleased(platform::Button::Escape),
			platform::getTypedInput(), deltaTime);
	}
	else
	{
		runGameplay(deltaTime, renderer, tileRenderer, playerRenderer, itemRenderer, npcRenderer);
	}



	renderer.flush();

	return true;
#pragma endregion

}

void closeGame()
{

	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
