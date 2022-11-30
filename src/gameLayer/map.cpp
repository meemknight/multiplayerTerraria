#include <map.h>
#include <PerlinNoise.hpp>
#include <random>

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

	if (t.isTree())
	{
		bool leftMiddleAir = (x > 0 && unsafeGet(x - 1, y).isAir());
		bool rightMiddleAir = (x < mapSize.x - 1 && unsafeGet(x + 1, y).isAir());
		bool topMiddleAir = (y > 0 && unsafeGet(x, y - 1).isAir());
		bool bottomMiddleAir = (y < mapSize.y - 1 && unsafeGet(x, y + 1).isAir());

		bool leftMiddleTree = (x > 0 && unsafeGet(x - 1, y).isTree());
		bool rightMiddleTree = (x < mapSize.x - 1 && unsafeGet(x + 1, y).isTree());
		bool topMiddleTree = (y > 0 && unsafeGet(x, y - 1).isTree());
		bool bottomMiddleTree = (y < mapSize.y - 1 && unsafeGet(x, y + 1).isTree());

		//if (!bottomMiddleAir)
		{
			if (bottomMiddleTree)
			{
				if (leftMiddleTree && rightMiddleTree && topMiddleTree)
				{
					//both branches uncut
					t.variationX = 4;
					t.variationY = 6;
				}
				else if (rightMiddleTree && topMiddleTree)
				{
					//both branches
					t.variationX = 0;
					t.variationY = 6;
				}
				else if (leftMiddleTree && topMiddleTree)
				{
					//roots uncut
					t.variationX = 3;
					t.variationY = 6;
				}
				else if (topMiddleTree)
				{
					//uncut
					t.variationX = 0;
					t.variationY = 0;
				}
				else
				{
					//todo branches
					//cut
					t.variationX = 5;
					t.variationY = 0;
				}
			}
			else if (!bottomMiddleAir)
			{
				if (leftMiddleTree && rightMiddleTree && topMiddleTree)
				{
					//both roots uncut
					t.variationX = 4;
					t.variationY = 6;
				}
				else if (rightMiddleTree && topMiddleTree)
				{
					//roots uncut
					t.variationX = 0;
					t.variationY = 6;
				}
				else if (leftMiddleTree && topMiddleTree)
				{
					//roots uncut
					t.variationX = 3;
					t.variationY = 6;
				}
				else if (topMiddleTree)
				{
					//uncut
					t.variationX = 0;
					t.variationY = 0;
				}
				else
				{
					//cut
					t.variationX = 0;
					t.variationY = 9;
				}
			}
			else 
			{
				if (rightMiddleTree)
				{
					//branches
					t.variationX = 3;
					t.variationY = 0;
				}
				else
				{
					t.variationX = 4;
					t.variationY = 3;
				}

			}

		}

	}else
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
			leftBottomAir = false;
			rightBottomAir = false;
			bottomMiddleAir = false;
		}

		if (y == 0)
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

		if (!t.isGrass())
		{
			leftTopDirt = (x > 0 && y > 0 && unsafeGet(x - 1, y - 1).isDirtOrGrass());
			leftMiddleDirt = (x > 0 && unsafeGet(x - 1, y).isDirtOrGrass());
			leftBottomDirt = (x > 0 && y < mapSize.y - 1 && unsafeGet(x - 1, y + 1).isDirtOrGrass());

			rightTopDirt = (x < mapSize.x - 1 && y > 0 && unsafeGet(x + 1, y - 1).isDirtOrGrass());
			rightMiddleDirt = (x < mapSize.x - 1 && unsafeGet(x + 1, y).isDirtOrGrass());
			rightBottomDirt = (x < mapSize.x - 1 && y < mapSize.y - 1 && unsafeGet(x + 1, y + 1).isDirtOrGrass());

			topMiddleDirt = (y > 0 && unsafeGet(x, y - 1).isDirtOrGrass());
			bottomMiddleDirt = (y < mapSize.y - 1 && unsafeGet(x, y + 1).isDirtOrGrass());
		}
		else
		{
			//leftTopDirt = (x > 0 && y > 0 && unsafeGet(x - 1, y - 1).isGrass());
			//leftMiddleDirt = (x > 0 && unsafeGet(x - 1, y).isGrass());
			//leftBottomDirt = (x > 0 && y < mapSize.y - 1 && unsafeGet(x - 1, y + 1).isGrass());

			//rightTopDirt = (x < mapSize.x - 1 && y > 0 && unsafeGet(x + 1, y - 1).isGrass());
			//rightMiddleDirt = (x < mapSize.x - 1 && unsafeGet(x + 1, y).isGrass());
			//rightBottomDirt = (x < mapSize.x - 1 && y < mapSize.y - 1 && unsafeGet(x + 1, y + 1).isGrass());

			//topMiddleDirt = (y > 0 && unsafeGet(x, y - 1).isGrass());
			//bottomMiddleDirt = (y < mapSize.y - 1 && unsafeGet(x, y + 1).isGrass());
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

			leftTopDirt =		0;
			leftMiddleDirt =	0;
			leftBottomDirt =	0;
			rightTopDirt =		0;
			rightMiddleDirt =	0;
			rightBottomDirt =	0;
			topMiddleDirt =		0;
			bottomMiddleDirt =	0;

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

			if (y == 0)
			{
				leftTopDirt = false;
				rightTopDirt = false;
				topMiddleDirt = false;
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
			else
			{
				t.variationX = 1;
				t.variationY = 2;
			}
		}
		else
		{
			//it is fully surounded by other blocks
			
			if (t.isGrass())
			{
				bool leftTopGrass = (x > 0 && y > 0 && unsafeGet(x - 1, y - 1).isGrass());
				bool leftMiddleGrass = (x > 0 && unsafeGet(x - 1, y).isGrass());
				bool leftBottomGrass = (x > 0 && y < mapSize.y - 1 && unsafeGet(x - 1, y + 1).isGrass());

				bool rightTopGrass = (x < mapSize.x - 1 && y > 0 && unsafeGet(x + 1, y - 1).isGrass());
				bool rightMiddleGrass = (x < mapSize.x - 1 && unsafeGet(x + 1, y).isGrass());
				bool rightBottomGrass = (x < mapSize.x - 1 && y < mapSize.y - 1 && unsafeGet(x + 1, y + 1).isGrass());

				bool topMiddleGrass = (y > 0 && unsafeGet(x, y - 1).isGrass());
				bool bottomMiddleGrass = (y < mapSize.y - 1 && unsafeGet(x, y + 1).isGrass());

				if (leftMiddleGrass && rightMiddleGrass && topMiddleGrass && bottomMiddleGrass)
				{
					t.variationX = 2;
					t.variationY = 1;
				}else
				//if (leftMiddleGrass && topMiddleGrass && rightMiddleGrass)
				//{
				//	t.variationX = 6;
				//	t.variationY = 1;
				//}else if (leftMiddleGrass && bottomMiddleGrass && rightMiddleGrass)
				//{
				//	t.variationX = 6;
				//	t.variationY = 2;
				//}else if (topMiddleGrass && bottomMiddleGrass && rightMiddleGrass)
				//{
				//	t.variationX = 11;
				//	t.variationY = 0;
				//}else if (topMiddleGrass && bottomMiddleGrass && leftMiddleGrass)
				//{
				//	t.variationX = 10;
				//	t.variationY = 0;
				//}else
				if (leftMiddleGrass && topMiddleGrass)
				{
					t.variationX = 3;
					t.variationY = 6;
				}
				else if (rightMiddleGrass && topMiddleGrass)
				{
					t.variationX = 2;
					t.variationY = 6;
				}
				else if (rightMiddleGrass && bottomMiddleGrass)
				{
					t.variationX = 2;
					t.variationY = 5;
				}
				else if (leftMiddleGrass && bottomMiddleGrass)
				{
					t.variationX = 3;
					t.variationY = 5;
				}
				else if (leftMiddleGrass)
				{
					t.variationX = 11;
					t.variationY = 17;
				}
				else if (rightMiddleGrass)
				{
					t.variationX = 8;
					t.variationY = 17;
				}
				else if (bottomMiddleGrass)
				{
					t.variationX = 11;
					t.variationY = 17;
				}
				else if (topMiddleGrass)
				{
					t.variationX = 11;
					t.variationY = 18;
				}
				else
				{
					goto noGrassCouldBePlaced;
				}


			}
			else
			{
				noGrassCouldBePlaced:

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
					t.variationX = 9;
					t.variationY = 7;
				}
				else if (rightMiddleDirt)
				{
					t.variationX = 8;
					t.variationY = 7;
				}
				else if (topMiddleDirt)
				{
					t.variationX = 8;
					t.variationY = 6;
				}
				else if (bottomMiddleDirt)
				{
					t.variationX = 8;
					t.variationY = 5;
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
					}
					else if (leftBottomAir && rightBottomAir)
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

	const int oceanLine = 230;
	const int dirtLayerSizeMin = 75;
	const int dirtLayerSizeMax = 120 - dirtLayerSizeMin;

	const float mountainMaxHeight = 60;
	const float valeyMaxDepth = 30;
	const int grassEnd = oceanLine + valeyMaxDepth + 30;

	const float holeChance = 0.3f;
	const float holeChanceMax = 0.35f;
	const float mediumHoleChance = 0.2;
	const float smallHoleChance = 0.2;
	const float stonePatchChance = 0.2;
	const float copperChance = 0.2;
	const float ironChance = 0.1;
	const float goldChance = 0.12;
	const float silverChance = 0.1;


	const int copperMaxDepth = oceanLine + 120;

	const int goldMinDepth = copperMaxDepth + 50;
	const int silverMinDepth = copperMaxDepth + 20;


	const float treeChance = 0.1;


	const int montainStart = oceanLine - mountainMaxHeight;

	m.create(mapW, mapH);
	float *horizonLine = 0;
	float *stoneLine = 0;

	{
		FastNoiseSIMD *horizonNoiseSet = FastNoiseSIMD::NewFastNoiseSIMD(seed);

		float scale = 1.5;
		horizonNoiseSet->SetAxisScales(scale, scale, scale);
		horizonNoiseSet->SetFrequency(0.030);
		horizonNoiseSet->SetFractalOctaves(5);

		horizonLine = horizonNoiseSet->GetPerlinFractalSet(0, 0, 0, mapW, 1, 1);
		stoneLine = horizonNoiseSet->GetPerlinFractalSet(0, 0, 45, mapW, 1, 1);
		delete horizonNoiseSet;
	}


	auto getHorizonValue = [&](int x) -> int
	{
		return int(oceanLine - (horizonLine[x] * (mountainMaxHeight + valeyMaxDepth) - valeyMaxDepth));
	};

	auto getStoneValue = [&](int x) -> int
	{
		return int(oceanLine - stoneLine[x] * dirtLayerSizeMax + dirtLayerSizeMin);
	};

	auto lerp = [](float a, float b, float v)
	{
		return a * (v - 1) + b * v;
	};

#pragma region horizon line
	for (int j = 0; j < mapH; j++)
	{
		for (int i = 0; i < mapW; i++)
		{
			auto horizon = getHorizonValue(i);

			if (j > getStoneValue(i))
			{
				m.unsafeGet(i, j).type = Tile::stone;
			}
			else
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
	FastNoiseSIMD::FreeNoiseSet(horizonLine);

#pragma endregion

#pragma region holes

	//big holes
	if (1)
	{
		siv::BasicPerlinNoise<float> noise;
		noise.reseed((uint32_t)seed);

		const float holeNoiseSize = 60.f;

		auto getHole = [&](int x, int y) -> bool
		{
			return noise.accumulatedOctaveNoise2D_0_1(x / holeNoiseSize, y / holeNoiseSize, 2.f) <
				lerp(holeChance, holeChanceMax, std::sqrt((float)y / mapH));
		};

		for (int j = montainStart; j < mapH; j++)
		{
			for (int i = 0; i < mapW; i++)
			{
				if (getHole(i, j))
				{
					m.safeGet(i, j).type = Tile::none;
				}
			}
		}
	}

	//medium holes
	if (1)
	{
		siv::BasicPerlinNoise<float> noise;
		noise.reseed((uint32_t)seed);

		const float holeNoiseSize = 20.f;

		auto getHole = [&](int x, int y) -> bool
		{
			return noise.accumulatedOctaveNoise2D_0_1(x / holeNoiseSize, y / holeNoiseSize, 2.f) < mediumHoleChance;
		};

		for (int j = montainStart; j < mapH; j++)
		{
			for (int i = 0; i < mapW; i++)
			{
				if (getHole(i, j))
				{
					m.safeGet(i, j).type = Tile::none;
				}
			}
		}
	}

	//small holes
	if (1)
	{
		siv::BasicPerlinNoise<float> noise;
		noise.reseed((uint32_t)seed);

		const float holeNoiseSize = 8.f;

		auto getHole = [&](int x, int y) -> bool
		{
			return noise.accumulatedOctaveNoise2D_0_1(x / holeNoiseSize, y / holeNoiseSize, 2.f) < smallHoleChance;
		};

		for (int j = montainStart; j < mapH; j++)
		{
			for (int i = 0; i < mapW; i++)
			{
				if (getHole(i, j))
				{
					m.safeGet(i, j).type = Tile::none;
				}
			}
		}
	}
#pragma endregion

#pragma region patches

	//small stone patches
	if (1)
	{
		siv::BasicPerlinNoise<float> noise;
		noise.reseed((uint32_t)seed + 2);

		const float stonePatchSize = 8.f;

		auto getStonePatch = [&](int x, int y) -> bool
		{
			return noise.accumulatedOctaveNoise2D_0_1(x / stonePatchSize, y / stonePatchSize, 2.f) < stonePatchChance;
		};

		for (int j = montainStart; j < mapH; j++)
		{
			for (int i = 0; i < mapW; i++)
			{
				if (!m.safeGet(i, j).isNone() && getStonePatch(i, j))
				{
					m.safeGet(i, j).type = Tile::stone;
				}
			}
		}
	}

	//small copper patches
	if (1)
	{
		siv::BasicPerlinNoise<float> noise;
		noise.reseed((uint32_t)seed + 100);

		const float stonePatchSize = 10.f;

		auto getCopperPatch = [&](int x, int y) -> bool
		{
			if (y > copperMaxDepth)
			{
				return noise.accumulatedOctaveNoise2D_0_1(x / stonePatchSize, y / stonePatchSize, 2.f) < copperChance * 0.3;
			}
			else
			{
				return noise.accumulatedOctaveNoise2D_0_1(x / stonePatchSize, y / stonePatchSize, 2.f) < copperChance;
			}
		};

		for (int j = montainStart; j < mapH; j++)
		{
			for (int i = 0; i < mapW; i++)
			{
				if (!m.safeGet(i, j).isNone() && getCopperPatch(i, j))
				{
					m.safeGet(i, j).type = Tile::copper;
				}
			}
		}
	}

	//small iron patches
	if (1)
	{
		siv::BasicPerlinNoise<float> noise;
		noise.reseed((uint32_t)seed + 140);

		const float stonePatchSize = 10.f;

		auto getIronPatch = [&](int x, int y) -> bool
		{
			return noise.accumulatedOctaveNoise2D_0_1(x / stonePatchSize, y / stonePatchSize, 2.f) < ironChance;
		};

		for (int j = montainStart; j < mapH; j++)
		{
			for (int i = 0; i < mapW; i++)
			{
				if (!m.safeGet(i, j).isNone() && getIronPatch(i, j))
				{
					m.safeGet(i, j).type = Tile::iron;
				}
			}
		}
	}

	//small gold patches
	if (1)
	{
		siv::BasicPerlinNoise<float> noise;
		noise.reseed((uint32_t)seed + 140);

		const float stonePatchSize = 17.f;

		auto getGoldPatch = [&](int x, int y) -> bool
		{
			return noise.accumulatedOctaveNoise2D_0_1(x / stonePatchSize, y / stonePatchSize, 2.f) < goldChance;
		};

		for (int j = goldMinDepth; j < mapH; j++)
		{
			for (int i = 0; i < mapW; i++)
			{
				if (!m.safeGet(i, j).isNone() && getGoldPatch(i, j))
				{
					m.safeGet(i, j).type = Tile::gold;
				}
			}
		}
	}

	//small silver patches
	if (1)
	{
		siv::BasicPerlinNoise<float> noise;
		noise.reseed((uint32_t)seed + 140);

		const float stonePatchSize = 3.f;

		auto getSilverPatch = [&](int x, int y) -> bool
		{
			return noise.accumulatedOctaveNoise2D_0_1(x / stonePatchSize, y / stonePatchSize, 3.f) < silverChance;
		};

		for (int j = silverMinDepth; j < mapH; j++)
		{
			for (int i = 0; i < mapW; i++)
			{
				if (!m.safeGet(i, j).isNone() && getSilverPatch(i, j))
				{
					m.safeGet(i, j).type = Tile::silver;
				}
			}
		}
	}

#pragma endregion

#pragma region add grass
	for (int j = 1; j < grassEnd; j++)
	{
		for (int i = 0; i < mapW; i++)
		{
			if (
				m.safeGet(i, j).type == Tile::dirt &&
				(
				m.unsafeGet(i, j - 1).isAir() ||
				m.unsafeGet(i, j + 1).isAir() ||
				(i > 0 && m.unsafeGet(i - 1, j).isAir()) ||
				(i < mapW - 1 && m.unsafeGet(i + 1, j).isAir()) ||
				(i > 0 && m.unsafeGet(i - 1, j - 1).isAir()) ||
				(i < mapW - 1 && m.unsafeGet(i + 1, j - 1).isAir()) ||
				(i > 0 && m.unsafeGet(i - 1, j + 1).isAir()) ||
				(i < mapW - 1 && m.unsafeGet(i + 1, j + 1).isAir())
				))
			{
				m.safeGet(i, j).type = Tile::grass;
			}



		}
	}
#pragma endregion

#pragma region add trees

	std::mt19937 randomDevice((unsigned int)seed);
	std::uniform_real perc(0.f, 1.f);

	auto chance = [&](float percent)
	{
		return perc(randomDevice) < percent;
	};

	auto randomVal = [&](int min, int max)
	{
		std::uniform_int dist(min, max);
		return dist(randomDevice);
	};

	for (int x = 0; x < mapW; x++)
	{

		if (chance(treeChance))
		{

			for (int y = 0; y < mapH; y++)
			{
				auto t = m.unsafeGet(x, y);

				if (t.isGrass())
				{
					//start spawnTree;
					m.safeGet(x, y - 1).type = Tile::tree;
					if (chance(0.2))
					{
						auto &b = m.safeGet(x + 1, y - 1);
						if (b.isAir())
						{
							b.type = Tile::tree;
						}
					}

					if (chance(0.2))
					{
						auto &b = m.safeGet(x - 1, y);
						if (b.isAir())
						{
							b.type = Tile::tree;
						}
					}


					int h = randomVal(3, 8);
					bool placedBranch = true;
					for (int t = 2; t < h; t++)
					{
						auto &b = m.safeGet(x, y - t);
						if (b.isAir())
						{
							b.type = Tile::tree;
						}
						else
						{
							break; //early break
						}

						if (placedBranch)
						{
							placedBranch = false;
						}else
						{
							if (chance(0.1))
							{
								auto &b = m.safeGet(x + 1, y - t);
								if (b.isAir())
								{
									b.type = Tile::tree;
									placedBranch = true;
								}
							}

							if (chance(0.1))
							{
								auto &b = m.safeGet(x - 1, y - t);
								if (b.isAir())
								{
									b.type = Tile::tree;
									placedBranch = true;
								}
							}
						}


					}



				}
				else if (!t.isAir())
				{
					break;
				}

			}


			//skip two
			x+=2;
		}


	}

#pragma endregion



	m.bakeEntireMap();


}
