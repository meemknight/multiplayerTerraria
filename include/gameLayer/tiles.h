#pragma once

#include <glm/vec3.hpp>

struct Tile
{
	enum
	{
		none = 0,
		dirt,
		stone,
		grass,
		tree,
		iron,
		copper,
		gold,
		silver,
		woddenPlank,
		TILES_COUNT
	};

	Tile() {};
	Tile(short unsigned int type):type(type) {};

	short unsigned int type = 0;
	unsigned char variationX = 0;
	unsigned char variationY = 0;

	static int fileId[TILES_COUNT];
	static glm::ivec3 tilesColor[TILES_COUNT];

	bool isAir();
	bool isNone();
	bool isDirt();
	bool isDirtOrGrass();
	bool isGrass();
};


