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
	auto viewRect = renderer.getViewRect();

	glm::ivec2 minV = {viewRect.x, viewRect.y};
	glm::ivec2 maxV = maxV + glm::ivec2{viewRect.z+1, viewRect.w+1};

	minV = glm::max(minV, {0,0});
	maxV = glm::min(maxV, {map.mapSize.x,map.mapSize.y});

	minV = {0,0};
	maxV = {map.mapSize.x,map.mapSize.y};

	for (int j = minV.y; j < maxV.y; j++)
		for (int i = minV.x; i < maxV.x; i++)
		{
			auto &t = map.safeGet(i, j);

			if (t.type == Tile::none) { continue; }

			auto &s = this->sprites[t.type];

			int tCoordsX = t.variationX;
			int tCoordsY = t.variationY;

			renderer.renderRectangle({i, j, 1, 1}, {}, {},
				s.t, s.getTextureCoords(tCoordsX, tCoordsY));
		}

}
