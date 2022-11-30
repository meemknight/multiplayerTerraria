#pragma once



struct Tiles
{
	enum
	{
		none,
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

	static int fileId[TILES_COUNT];
};


