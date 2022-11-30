#include <map.h>

void Map::create(int x, int y)
{

	cleanup();

	mapSize = {15,10};

	tiles.resize(mapSize.x * mapSize.y);

	safeGet(3, 3) = Tile::stone;
	safeGet(4, 3) = Tile::stone;
	safeGet(4, 4) = Tile::stone;
	safeGet(4, 2) = Tile::stone;
	safeGet(4, 10) = Tile::stone;
	safeGet(5, 3) = Tile::stone;
	safeGet(6, 7) = Tile::stone;
	
	safeGet(8, 3) = Tile::stone;
	safeGet(9, 3) = Tile::stone;
	safeGet(10, 3) = Tile::stone;

	safeGet(13, 3) = Tile::stone;
	safeGet(13, 4) = Tile::stone;
	safeGet(13, 5) = Tile::stone;

	safeGet(2, 7) = Tile::stone;
	safeGet(2, 8) = Tile::stone;
	safeGet(3, 7) = Tile::stone;
	safeGet(3, 8) = Tile::stone;


	bakeEntireMap();
}

void Map::cleanup()
{
	*this = Map{};
}

Tile stub = {};
Tile &Map::safeGet(int x, int y)
{
	if (x < 0 || y < 0 || x >= mapSize.x || y >= mapSize.y)
	{
		stub = {};
		return stub;
	}
	
	return unsafeGet(x, y);
}

Tile &Map::unsafeGet(int x, int y)
{
	return tiles[x + y * mapSize.x];
}

void Map::bakeBlockUnsafe(int x, int y)
{
	auto &t = this->unsafeGet(x, y);

	if (!t.isNone())
	{


		//bool leftTopAir = (x > 0 && y > 0 && unsafeGet(x - 1, y - 1).isAir());
		bool leftMiddleAir = (x > 0 && unsafeGet(x - 1, y).isAir());
		//bool leftBottomAir = (x > 0 && y < mapSize.y - 1 && unsafeGet(x - 1, y + 1).isAir());

		//bool rightTopAir = (x < mapSize.x-1 && y > 0 && unsafeGet(x + 1, y - 1).isAir());
		bool rightMiddleAir = (x < mapSize.x - 1 && unsafeGet(x + 1, y).isAir());
		//bool rightBottomAir = (x < mapSize.x - 1 && y < mapSize.y - 1 && unsafeGet(x + 1, y + 1).isAir());

		bool topMiddleAir = (y > 0 && unsafeGet(x, y - 1).isAir());
		bool bottomMiddleAir = (y < mapSize.y-1 && unsafeGet(x, y + 1).isAir());

		if ( leftMiddleAir 
			&& rightMiddleAir 
			&& topMiddleAir && bottomMiddleAir
			)
		{
			//block fulli surounded by air
			t.variationX = 9;
			t.variationY = 3;
		}
		else if (
			rightMiddleAir
			&& topMiddleAir && bottomMiddleAir
			&& !leftMiddleAir
			)
		{
			//block fully surounded by air except left
			t.variationX = 12;
			t.variationY = 0;
		}
		else if (
			leftMiddleAir
			&& topMiddleAir && bottomMiddleAir
			&& !rightMiddleAir
			)
		{
			//block fully surounded by air except right
			t.variationX = 9;
			t.variationY = 0;
		}
		else if (
			leftMiddleAir
			&& rightMiddleAir
			&& bottomMiddleAir
			&& !topMiddleAir
			)
		{
			//block fully surounded by air except top
			t.variationX = 6;
			t.variationY = 3;
		}
		else if (
			leftMiddleAir
			&& rightMiddleAir
			&& topMiddleAir
			&& !bottomMiddleAir
			)
		{
			//block fully surounded by air except bottom
			t.variationX = 6;
			t.variationY = 0;
		}
		else if (
			topMiddleAir && bottomMiddleAir
			&& !leftMiddleAir
			&& !rightMiddleAir
			)
		{
			//block fully surounded by air except left right
			t.variationX = 6;
			t.variationY = 4;
		}
		else if (
			!topMiddleAir && !bottomMiddleAir
			&& leftMiddleAir
			&& rightMiddleAir
			)
		{
			//block fully surounded by air except top bottom
			t.variationX = 5;
			t.variationY = 0;
		}
		else if (
			leftMiddleAir
			&& !rightMiddleAir
			&& topMiddleAir && !bottomMiddleAir)
		{
			//cadran 2
			t.variationX = 0;
			t.variationY = 3;
		}
		else if (
			!leftMiddleAir
			&& rightMiddleAir
			&& topMiddleAir && !bottomMiddleAir)
		{
			//cadran 1
			t.variationX = 1;
			t.variationY = 3;
		}
		else if (
			leftMiddleAir
			&& !rightMiddleAir
			&& !topMiddleAir && bottomMiddleAir)
		{
			//cadran 3
			t.variationX = 0;
			t.variationY = 4;
		}
		else if (
			!leftMiddleAir
			&& rightMiddleAir
			&& !topMiddleAir && bottomMiddleAir)
		{
			//cadran 4
			t.variationX = 1;
			t.variationY = 4;
		}
		else if (leftMiddleAir)
		{
			t.variationX = 0;
			t.variationY = 0;
		}
		else if (rightMiddleAir)
		{
			t.variationX = 4;
			t.variationY = 0;
		}
		else if (topMiddleAir)
		{
			t.variationX = 1;
			t.variationY = 0;
		}
		else if (bottomMiddleAir)
		{
			t.variationX = 1;
			t.variationY = 2;
		}
		else
		{
			t.variationX = 1; //it is surounded by other blocks
			t.variationY = 1;
		}
	}

	


}

void Map::bakeBlockAndNeighboursUnsafe(int x, int y)
{
	bakeBlockSafe(x - 1, y - 1);
	bakeBlockSafe(x    , y - 1);
	bakeBlockSafe(x + 1, y - 1);

	bakeBlockSafe(x - 1, y    );
	bakeBlockUnsafe(x,   y    ); //middle
	bakeBlockSafe(x + 1, y    );

	bakeBlockSafe(x - 1, y + 1);
	bakeBlockSafe(x    , y + 1);
	bakeBlockSafe(x + 1, y + 1);
}

void Map::bakeBlockSafe(int x, int y)
{
	if (x < 0 || y < 0 || x >= mapSize.x || y >= mapSize.y) { return; }

	bakeBlockUnsafe(x, y);
}

void Map::bakeEntireMap()
{
	for (int j = 0; j < mapSize.y; j++)
		for (int i = 0; i < mapSize.x; i++)
		{
			bakeBlockUnsafe(i, j);
		}

}
