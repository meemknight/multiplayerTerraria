#include "sprites.h"

void Sprite::load(const char *f)
{
	t.loadFromFile(f, true, true);
	size = t.GetSize();
}

void Sprite::load(const char *f, int x, int y)
{
	load(f);
	blockSize = {x,y};
}

glm::vec4 Sprite::getTextureCoords(int x, int y, bool flipX)
{
	glm::vec2 pixelSize = 1.f / (glm::vec2)size;

	glm::ivec2 begin = blockSize * glm::ivec2{x, y} + 2 * glm::ivec2{x,y};
	glm::ivec2 end = begin + blockSize; 

	glm::vec2 beginPixel = glm::vec2(begin) * pixelSize;
	glm::vec2 endPixel =   glm::vec2(end) * pixelSize;

	glm::vec4 noFlip = {beginPixel.x, 1-beginPixel.y, endPixel.x, 1-endPixel.y};

	if (flipX)
	{
		glm::vec4 flip = {noFlip.z, noFlip.y, noFlip.x, noFlip.w};
		return flip;
	}
	else
	{
		return noFlip;
	}
}
