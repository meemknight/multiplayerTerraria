#include <tileRenderer.h>
#include <string>


void TileRenderer::loadAll()
{
	for (int i = 1; i < Tile::TILES_COUNT; i++)
	{
		std::string f = RESOURCES_PATH "Tiles_" + std::to_string(Tile::fileId[i]) + ".png";

		sprites[i].load(f.c_str(), 16, 16);
	}

}

void TileRenderer::renderMap(gl2d::Renderer2D &renderer, Map &map)
{

	for (int j = 0; j < map.mapSize.y; j++)
		for (int i = 0; i < map.mapSize.x; i++)
		{
			auto &t = map.safeGet(i, j);

			if (t.type == Tile::none) { continue; }

			auto &s = this->sprites[t.type];

			int tCoordsX = t.variationX;
			int tCoordsY = t.variationY;

			renderer.renderRectangle({100 * i, 100 * j, 100, 100}, {}, {},
				s.t, s.getTextureCoords(tCoordsX, tCoordsY));
		}

}
