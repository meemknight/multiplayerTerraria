#pragma once

#include <vector>
#include "tiles.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <gl2d/gl2d.h>

struct Map
{

	std::vector<Tile> tiles = {};
	glm::ivec2 mapSize = {};

	void create(int x, int y);
	void cleanup();

	glm::ivec2 spawnPos = {};

	Tile &safeGet(int x, int y);
	Tile &unsafeGet(int x, int y);

	void unsafePlace(int x, int y, unsigned short type);

	void bakeBlockUnsafe(int x, int y);
	void bakeBlockAndNeighboursUnsafe(int x, int y);
	void bakeBlockSafe(int x, int y);

	void bakeEntireMap();

	void renderMapIntoTexture();

	GLuint texture = 0;
};


void generateMap(Map &m, int seed);