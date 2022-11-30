#include <tiles.h>


int Tile::fileId[Tile::TILES_COUNT] =
{
	-1,
	0,
	1,
	2,
	5,
	6,
	7,
	8,
	9,
	30 //wodden plank
};

bool Tile::isAir()
{
	return type == none;
}

bool Tile::isNone()
{
	return type == none;
}

bool Tile::isDirt()
{
	return type == dirt || type == grass;
}

bool Tile::isGrass()
{
	return type == grass;
}
