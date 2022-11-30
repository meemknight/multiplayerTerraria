#pragma once

#include "sprites.h"
#include "tiles.h"
#include "map.h"
#include <gl2d/gl2d.h>

struct TileRenderer
{

	Sprite sprites[Tile::TILES_COUNT];

	void loadAll();


	void renderMap(gl2d::Renderer2D &renderer, Map &map);

};