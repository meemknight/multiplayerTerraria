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

struct PlayerSkin
{
	glm::vec3 eyeColor = {0.2,0.5,0.7};
	glm::vec3 skinColor = {236 / 255.f, 161 / 255.f, 130 / 255.f};
	glm::vec3 hairColor = {55 / 255.f, 31 / 255.f, 24 / 255.f};
	glm::vec3 clothesColor = {189 / 255.f, 30 / 255.f, 73 / 255.f};
	glm::vec3 pantsColor = {33 / 255.f, 14 / 255.f, 30 / 255.f};

	bool hasClothes = true;
	bool hasPants = true;

	int hairType = 0;

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
		clothes,
		pants,
		BODY_PARTS_COUNT

	};

	void loadAll();

	void render(gl2d::Renderer2D &renderer, glm::vec2 pos, PlayerSkin &skin, bool movingRight);

};