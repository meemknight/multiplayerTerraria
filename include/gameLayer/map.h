#pragma once

#include <vector>
#include "tiles.h"
#include <glm/vec2.hpp>

struct Map
{

	std::vector<Tile> tiles = {};
	glm::ivec2 mapSize = {};

	void create(int x, int y);
	void cleanup();

	Tile &safeGet(int x, int y);
	Tile &unsafeGet(int x, int y);

	void bakeBlockUnsafe(int x, int y);
	void bakeBlockAndNeighboursUnsafe(int x, int y);
	void bakeBlockSafe(int x, int y);

	void bakeEntireMap();
};