#pragma once

#include <gl2d/gl2d.h>


struct Sprite
{
	Sprite() {};
	Sprite(const char *f) { load(f); }
	Sprite(const char *f, int x, int y) { load(f); blockSize = {x,y}; }
	Sprite(const char *f, glm::ivec2 blockSize):blockSize(blockSize) { load(f); }

	void load(const char *f);
	void load(const char *f, int x, int y);

	gl2d::Texture t = {};
	glm::ivec2 size = {};
	glm::ivec2 blockSize = {};
	
	glm::vec4 getTextureCoords(int x, int y, bool flipX = 0);


};