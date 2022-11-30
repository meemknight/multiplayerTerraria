#pragma once

#include "sprites.h"
#include "tiles.h"

struct TileRenderer
{

	Sprite sprites[Tiles::TILES_COUNT];

	void loadAll();


};