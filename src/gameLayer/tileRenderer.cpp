#include <tileRenderer.h>
#include <string>


void TileRenderer::loadAll()
{
	for (int i = 1; i < Tiles::TILES_COUNT; i++)
	{
		std::string f = RESOURCES_PATH "Tiles_" + std::to_string(Tiles::fileId[i]) + ".png";

		sprites[i].load(f.c_str(), 16, 16);
	}

}