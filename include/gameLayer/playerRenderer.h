#pragma once
#include "sprites.h"

struct BodyPart
{
	BodyPart() {};

	BodyPart(int textureId, glm::ivec2 atlasSize):textureId(textureId), atlasSize(atlasSize) {};

	int textureId = 0;
	glm::ivec2 atlasSize = {};
	
	Sprite s;
};

struct PlayerRenderer
{
	
	enum bodyParts
	{
		head,
		eyeWhite,
		eye,
		torso,
		rightArm,
		legs,
		BODY_PARTS_COUNT

	};

	void loadAll();

	void render(gl2d::Renderer2D &renderer, glm::vec2 pos);

};