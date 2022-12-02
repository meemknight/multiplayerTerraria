#pragma once
#include <gl2d/gl2d.h>

struct Item
{

	enum
	{
		none = 0,
		pickaxe,
		dirt,
		stone,
		woddenPlank,
		ITEMS_COUNT,
	};

	static int textureID[ITEMS_COUNT];

	unsigned short type = 0;

};


struct ItemRenderer
{

	gl2d::Texture textures[Item::ITEMS_COUNT] = {};

	void loadAll();

};