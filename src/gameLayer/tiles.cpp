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

glm::ivec3 Tile::tilesColor[Tile::TILES_COUNT] =
{
	{},
	{151,107,75}, //dirt
	{128,128,128}, //stone
	{28,216,94}, //grass
	{148,133,98}, //wood
	{189,159,139}, //iron
	{183,88,25}, //copper
	{231,213,65}, //gold
	{171,182,183}, //silver
	{151,107,75}, //woden plank

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
	return type == dirt;
}

bool Tile::isDirtOrGrass()
{
	return type == dirt || type == grass;
}


bool Tile::isGrass()
{
	return type == grass;
}
