#include <map.h>

void Map::create(int x, int y)
{

	cleanup();

	mapSize = {x, y};

	tiles.resize(mapSize.x * mapSize.y);

}

void Map::cleanup()
{
	glDeleteTextures(1, &texture);
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

		bool leftTopAir = (x > 0 && y > 0 && unsafeGet(x - 1, y - 1).isAir());
		bool leftMiddleAir = (x > 0 && unsafeGet(x - 1, y).isAir());
		bool leftBottomAir = (x > 0 && y < mapSize.y - 1 && unsafeGet(x - 1, y + 1).isAir());

		bool rightTopAir = (x < mapSize.x-1 && y > 0 && unsafeGet(x + 1, y - 1).isAir());
		bool rightMiddleAir = (x < mapSize.x - 1 && unsafeGet(x + 1, y).isAir());
		bool rightBottomAir = (x < mapSize.x - 1 && y < mapSize.y - 1 && unsafeGet(x + 1, y + 1).isAir());

		bool topMiddleAir = (y > 0 && unsafeGet(x, y - 1).isAir());
		bool bottomMiddleAir = (y < mapSize.y-1 && unsafeGet(x, y + 1).isAir());

		if (y == mapSize.y - 1)
		{
			leftTopAir = true;
			rightTopAir = true;
			topMiddleAir = true;
		}

		bool leftTopDirt = 0;
		bool leftMiddleDirt = 0;
		bool leftBottomDirt = 0;
		bool rightTopDirt = 0;
		bool rightMiddleDirt = 0;
		bool rightBottomDirt = 0;
		bool topMiddleDirt = 0;
		bool bottomMiddleDirt = 0;

		//if (!t.isGrass())
		{
			leftTopDirt = (x > 0 && y > 0 && unsafeGet(x - 1, y - 1).isDirt());
			leftMiddleDirt = (x > 0 && unsafeGet(x - 1, y).isDirt());
			leftBottomDirt = (x > 0 && y < mapSize.y - 1 && unsafeGet(x - 1, y + 1).isDirt());

			rightTopDirt = (x < mapSize.x - 1 && y > 0 && unsafeGet(x + 1, y - 1).isDirt());
			rightMiddleDirt = (x < mapSize.x - 1 && unsafeGet(x + 1, y).isDirt());
			rightBottomDirt = (x < mapSize.x - 1 && y < mapSize.y - 1 && unsafeGet(x + 1, y + 1).isDirt());

			topMiddleDirt = (y > 0 && unsafeGet(x, y - 1).isDirt());
			bottomMiddleDirt = (y < mapSize.y - 1 && unsafeGet(x, y + 1).isDirt());
		}

		if (t.type == Tile::dirt) 
		{
			leftTopDirt =		!leftTopDirt;
			leftMiddleDirt =	!leftMiddleDirt;
			leftBottomDirt =	!leftBottomDirt;
			rightTopDirt =		!rightTopDirt;
			rightMiddleDirt =	!rightMiddleDirt;
			rightBottomDirt =	!rightBottomDirt;
			topMiddleDirt =		!topMiddleDirt;
			bottomMiddleDirt =	!bottomMiddleDirt;

			if (x == 0)
			{
				leftTopDirt = false;
				leftBottomDirt = false;
				leftMiddleDirt = false;
			}

			if (x == mapSize.x-1)
			{
				rightTopDirt = false;
				rightBottomDirt = false;
				rightMiddleDirt = false;
			}

			if (y == mapSize.y - 1)
			{
				rightBottomDirt = false;
				bottomMiddleDirt = false;
				leftBottomDirt = false;
			}

		}


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
			if (leftMiddleDirt)
			{
				t.variationX = 0;
				t.variationY = 13;
			}
			else
			{
				t.variationX = 12;
				t.variationY = 0;
			}
		}
		else if (
			leftMiddleAir
			&& topMiddleAir && bottomMiddleAir
			&& !rightMiddleAir
			)
		{
			//block fully surounded by air except right

			if (rightMiddleDirt)
			{
				t.variationX = 3;
				t.variationY = 13;
			}
			else
			{
				t.variationX = 9;
				t.variationY = 0;
			}
		}
		else if (
			leftMiddleAir
			&& rightMiddleAir
			&& bottomMiddleAir
			&& !topMiddleAir
			)
		{
			//block fully surounded by air except top

			if (topMiddleDirt)
			{
				t.variationX = 6;
				t.variationY = 8;
			}
			else
			{
				t.variationX = 6;
				t.variationY = 3;
			}
			
		}
		else if (
			leftMiddleAir
			&& rightMiddleAir
			&& topMiddleAir
			&& !bottomMiddleAir
			)
		{
			//block fully surounded by air except bottom
			
			if (bottomMiddleDirt)
			{
				t.variationX = 6;
				t.variationY = 5;
			}
			else
			{
				t.variationX = 6;
				t.variationY = 0;
			}


		}
		else if (
			topMiddleAir && bottomMiddleAir
			&& !leftMiddleAir
			&& !rightMiddleAir
			)
		{
			//block fully surounded by air except left right
			if (leftMiddleDirt && rightMiddleDirt)
			{
				t.variationX = 9;
				t.variationY = 11;
			}
			else if (leftMiddleDirt)
			{
				t.variationX = 0;
				t.variationY = 14;
			}
			else if (rightMiddleDirt)
			{
				t.variationX = 3;
				t.variationY = 14;
			}
			else
			{
				t.variationX = 6;
				t.variationY = 4;
			}
			
		}
		else if (
			!topMiddleAir && !bottomMiddleAir
			&& leftMiddleAir
			&& rightMiddleAir
			)
		{
			//block fully surounded by air except top bottom
			if (topMiddleDirt && bottomMiddleDirt)
			{
				t.variationX = 6;
				t.variationY = 12;
			}
			else if (topMiddleDirt)
			{
				t.variationX = 6;
				t.variationY = 8;
			}
			else if (bottomMiddleDirt)
			{
				t.variationX = 6;
				t.variationY = 5;
			}
			else
			{
				t.variationX = 5;
				t.variationY = 0;
			}

			
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
			if (rightMiddleDirt)
			{
				t.variationX = 13;
				t.variationY = 2;
			}else
			{
				t.variationX = 0;
				t.variationY = 0;
			}
			
		}
		else if (rightMiddleAir)
		{
			
			if (leftMiddleDirt)
			{
				t.variationX = 13;
				t.variationY = 3;
			}
			else
			{
				t.variationX = 4;
				t.variationY = 0;
			}

		}
		else if (topMiddleAir)
		{
			if(bottomMiddleDirt)
			{
				t.variationX = 13;
				t.variationY = 0;
			}
			else
			{
				t.variationX = 1;
				t.variationY = 0;
			}
			
		}
		else if (bottomMiddleAir)
		{
			if(topMiddleDirt)
			{
				t.variationX = 13;
				t.variationY = 1;
			}
			else if (bottomMiddleDirt)
			{
				t.variationX = 1;
				t.variationY = 2;
			}
		}
		else
		{
			//it is fully surounded by other blocks

		

			if (
				leftMiddleDirt
				&& rightMiddleDirt
				&& topMiddleDirt
				&& bottomMiddleDirt
				)
			{
				//fully surounded by dirt
				t.variationX = 6;
				t.variationY = 11;
			}
			else if (!leftMiddleDirt
				&& rightMiddleDirt
				&& topMiddleDirt
				&& bottomMiddleDirt
				)
			{
				t.variationX = 12;
				t.variationY = 8;
			}
			else if (leftMiddleDirt
				&& !rightMiddleDirt
				&& topMiddleDirt
				&& bottomMiddleDirt
				)
			{
				t.variationX = 12;
				t.variationY = 5;
			}
			else if (leftMiddleDirt
				&& rightMiddleDirt
				&& !topMiddleDirt
				&& bottomMiddleDirt
				)
			{
				t.variationX = 11;
				t.variationY = 8;
			}
			else if (leftMiddleDirt
				&& rightMiddleDirt
				&& topMiddleDirt
				&& !bottomMiddleDirt
				)
			{
				t.variationX = 11;
				t.variationY = 5;
			}
			else if (!leftMiddleDirt
				&& rightMiddleDirt
				&& topMiddleDirt
				&& !bottomMiddleDirt
				)
			{
				//cadran 1
				t.variationX = 3;
				t.variationY = 5;
			}
			else if (leftMiddleDirt
				&& !rightMiddleDirt
				&& topMiddleDirt
				&& !bottomMiddleDirt
				)
			{
				//cadran 2
				t.variationX = 2;
				t.variationY = 5;
			}
			else if (leftMiddleDirt
				&& !rightMiddleDirt
				&& !topMiddleDirt
				&& bottomMiddleDirt
				)
			{
				//cadran 3
				t.variationX = 2;
				t.variationY = 6;
			}
			else if (!leftMiddleDirt
				&& rightMiddleDirt
				&& !topMiddleDirt
				&& bottomMiddleDirt
				)
			{
				//cadran 4
				t.variationX = 3;
				t.variationY = 6;
			}
			else if (!leftMiddleDirt
				&& !rightMiddleDirt
				&& topMiddleDirt
				&& bottomMiddleDirt)
			{
				//top bottom dirt
				t.variationX = 8;
				t.variationY = 10;
			}
			else if (
				leftMiddleDirt
				&& rightMiddleDirt
				&& !topMiddleDirt
				&& !bottomMiddleDirt
				)
			{
				//left right dirt
				t.variationX = 10;
				t.variationY = 7;
			}
			else if (leftMiddleDirt)
			{
				t.variationX = 10;
				t.variationY = 7;
			}else if (rightMiddleDirt)
			{
				t.variationX = 9;
				t.variationY = 7;
			}else if (topMiddleDirt)
			{
				t.variationX = 9;
				t.variationY = 8;
			}else if (bottomMiddleDirt)
			{
				t.variationX = 9;
				t.variationY = 7;
			}
			else //no dirt
			{
				if (leftTopAir && rightTopAir)
				{
					t.variationX = 6;
					t.variationY = 1;
				}
				else if (rightTopAir && rightBottomAir)
				{
					t.variationX = 11;
					t.variationY = 0;
				}
				else if (leftTopAir && leftBottomAir)
				{
					t.variationX = 10;
					t.variationY = 0;
				}else if (leftBottomAir && rightBottomAir)
				{
					t.variationX = 6;
					t.variationY = 2;
				}
				else
				{
					t.variationX = 1;
					t.variationY = 1;
				}
			}
			
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

void Map::renderMapIntoTexture()
{
	glDeleteTextures(1, &texture);

	unsigned char *pixels = new unsigned char[mapSize.x * mapSize.y * 4];

	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			glm::ivec3 b = Tile::tilesColor[unsafeGet(i, j).type];
			pixels[(i + j * mapSize.x) * 4 + 0] = b[0];
			pixels[(i + j * mapSize.x) * 4 + 1] = b[1];
			pixels[(i + j * mapSize.x) * 4 + 2] = b[2];
			pixels[(i + j * mapSize.x) * 4 + 3] = 255;

		}
	}

	gl2d::Texture t;
	t.createFromBuffer((char*)pixels, mapSize.x, mapSize.y, true);

	texture = t.id;

	delete[] pixels;
}

#include <FastNoiseSIMD.h>

void generateMap(Map &m, int seed)
{
	const int mapW = 600;
	const int mapH = 750;

	const int oceanLine = 330;
	const int dirtLayerSize = 15;
	const float mountainMaxHeight = 60;
	const float valeyMaxDepth = 30;
	const int grassEnd = oceanLine + 50;

	m.create(mapW, mapH);
	float *horizonLine = 0;

	{
		FastNoiseSIMD *horizonNoiseSet = FastNoiseSIMD::NewFastNoiseSIMD(seed);

		float scale = 1.5;
		horizonNoiseSet->SetAxisScales(scale, scale, scale);
		horizonNoiseSet->SetFrequency(0.030);
		horizonNoiseSet->SetFractalOctaves(5);

		horizonLine = horizonNoiseSet->GetPerlinFractalSet(0, 0, 0, mapW, 1, 1);
		delete horizonNoiseSet;
	}


	auto getHorizonValue = [&](int x) -> int
	{
		return int(oceanLine - (horizonLine[x] * (mountainMaxHeight + valeyMaxDepth) - valeyMaxDepth));
	};

#pragma region horizon line
	for (int j = 0; j < mapH; j++)
	{
		for (int i = 0; i < mapW; i++)
		{
			auto horizon = getHorizonValue(i);
			if (j > horizon)
			{
				m.unsafeGet(i, j).type = Tile::dirt;
			}
			else
			{
				m.unsafeGet(i, j).type = 0;

			}

		}
	}
#pragma endregion

#pragma region add grass
	for (int j = 1; j < mapH-1; j++)
	{
		for (int i = 0; i < mapW; i++)
		{
			if (
				m.safeGet(i, j).type == Tile::dirt &&
				(
				m.unsafeGet(i, j - 1).isAir() ||
				m.unsafeGet(i, j + 1).isAir() ||
				(i > 0 && m.unsafeGet(i - 1, j).isAir()) ||
				(i < mapW-1 && m.unsafeGet(i + 1, j).isAir()) ||
				(i > 0 && m.unsafeGet(i - 1, j-1).isAir()) ||
				(i < mapW - 1 && m.unsafeGet(i + 1, j-1).isAir())||
				(i > 0 && m.unsafeGet(i - 1, j+1).isAir()) ||
				(i < mapW - 1 && m.unsafeGet(i + 1, j+1).isAir())
				))
			{
				m.safeGet(i, j).type = Tile::grass;
			}



		}
	}
#pragma endregion


	m.bakeEntireMap();


}
