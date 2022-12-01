#include <tileRenderer.h>
#include <string>


void TileRenderer::loadAll()
{
	for (int i = 1; i < Tile::TILES_COUNT; i++)
	{
		std::string f = RESOURCES_PATH "Tiles_" + std::to_string(Tile::fileId[i]) + ".png";

		if (i == Tile::tree)
		{
			sprites[i].load(f.c_str(), 20, 20);
		}
		else if (i == Tile::treeTop)
		{
			sprites[i].load(RESOURCES_PATH "Tree_Tops_0.png", 80, 80);
		}
		else
		{
			sprites[i].load(f.c_str(), 16, 16);
		}

	}

}

void TileRenderer::renderMap(gl2d::Renderer2D &renderer, Map &map)
{
	auto viewRect = renderer.getViewRect();

	glm::ivec2 minV = {viewRect.x-2, viewRect.y-2};
	glm::ivec2 maxV = minV + glm::ivec2{viewRect.z+4, viewRect.w+4};

	minV = glm::max(minV, {0,0});
	maxV = glm::min(maxV, {map.mapSize.x,map.mapSize.y});


	for (int j = minV.y; j < maxV.y; j++)
		for (int i = minV.x; i < maxV.x; i++)
		{
			auto &t = map.safeGet(i, j);

			if (t.type == Tile::none) { continue; }

			auto &s = this->sprites[t.type];

			int tCoordsX = t.variationX;
			int tCoordsY = t.variationY;

			if (t.type == Tile::treeTop)
			{
				renderer.renderRectangle({
					i - 1.5 - 1 / 16.f,
					j - 3 + 2.f / 16.f,
					4 + 2/16.f, 
					4}, {}, {},
					s.t, s.getTextureCoords(tCoordsX, tCoordsY));
			}else
			if (t.isTree())
			{
				if (
					(t.variationX == 3 &&
					t.variationY == 0) ||
					(t.variationX == 2 &&
					t.variationY == 6)
					)
				{
					renderer.renderRectangle({i + 4.f / 16.f, j + 2.f / 16.f, 1, 1}, {}, {},
						s.t, s.getTextureCoords(tCoordsX, tCoordsY));
				}
				else if (
					(
					t.variationX == 4 &&
					t.variationY == 3) 
					|| (t.variationX == 1 &&
					t.variationY == 6)
					)
				{
					renderer.renderRectangle({i - 4.f / 16.f, j + 2.f / 16.f, 1, 1}, {}, {},
						s.t, s.getTextureCoords(tCoordsX, tCoordsY));
				}
				else
				{
					renderer.renderRectangle({i, j + 2.f / 16.f, 1, 1}, {}, {},
						s.t, s.getTextureCoords(tCoordsX, tCoordsY));
				}
			
			}
			else
			{
				renderer.renderRectangle({i, j, 1, 1}, {}, {},
					s.t, s.getTextureCoords(tCoordsX, tCoordsY));
			}

			
		}

}
