#pragma once
#include <gameLayer.h>
#include <algorithm>
#include <gl2d/gl2d.h>
#include <iostream>
#include <sstream>
#include "sprites.h"
#include <tileRenderer.h>
#include "map.h"
#include "playerRenderer.h"
#include "imgui.h"
#include <entity.h>
#include <platformInput.h>
#include <items.h>
#include <npcs.h>


//void initGameplay(PlayerSkin playerSkin);

void runGameplay(float deltaTime, gl2d::Renderer2D &renderer, TileRenderer &tileRenderer,
	PlayerRenderer &playerRenderer, ItemRenderer &itemRenderer, NpcRenderer &npcRenderer);

bool joinServer(PlayerSkin playerSkin, const char *ip);
