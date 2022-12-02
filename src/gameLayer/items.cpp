#include <items.h>
#include <string>

int Item::textureID[Item::ITEMS_COUNT] = {
	0,
	1,
	2,
	3,
	9
};

void ItemRenderer::loadAll()
{
	for (int i = 1; i < Item::ITEMS_COUNT; i++)
	{
		std::string f = RESOURCES_PATH "items/item_" + std::to_string(Item::textureID[i]) + ".png";
		textures[i].loadFromFile(f.c_str(), true, true);


	}


}
