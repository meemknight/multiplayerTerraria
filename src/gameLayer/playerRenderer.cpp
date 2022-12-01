#include <playerRenderer.h>
#include <string>

BodyPart loadedBodyParts[PlayerRenderer::bodyParts::BODY_PARTS_COUNT] = {
	{0, {38, 54}},
	{1, {38, 54}},
	{2, {38, 54}},
	{3, {38, 54}}, //torso
	{7, {38, 54}}, //right arm
	{10, {38, 54}}, //feet arm
	{6, {38, 54}}, //clothes
	{11, {38, 54}}, //pants


};

Sprite hairSprites[20];



void PlayerRenderer::loadAll()
{
	for (int i = 0; i < bodyParts::BODY_PARTS_COUNT; i++)
	{
		std::string f = RESOURCES_PATH "Player_0_" + std::to_string(loadedBodyParts[i].textureId) + ".png";
		loadedBodyParts[i].s.load(f.c_str(), loadedBodyParts[i].atlasSize.x, loadedBodyParts[i].atlasSize.y);
	}

	for (int i = 0; i < 20; i++)
	{
		std::string f = RESOURCES_PATH "/playerHair/Player_Hair_" + std::to_string(i+1) + ".png";
		hairSprites[i].load(f.c_str(), 38, 54);
	}


}

void PlayerRenderer::render(gl2d::Renderer2D &renderer, glm::vec2 pos, PlayerSkin &skin, bool movingRight)
{

	float pixelSize = 1.f / 16;
	glm::vec2 pixelSize2{pixelSize, pixelSize};

	pos.x -= pixelSize * 8;
	pos.y -= pixelSize * 4;


	auto drawPart = [&](int i, int x, int y, glm::vec3 color)
	{
		auto &p = loadedBodyParts[i];
		renderer.renderRectangle({pos, pixelSize2 * glm::vec2(p.atlasSize)},
			glm::vec4(color, 1.f), {}, {},
			p.s.t, p.s.getTextureCoords(x, y, !movingRight));
	};

	auto drawHair = [&](int i, int x, int y, glm::vec3 color)
	{
		auto &p = hairSprites[i];
		renderer.renderRectangle({pos, pixelSize2 * glm::vec2(p.blockSize)},
			glm::vec4(color, 1.f), {}, {},
			p.t, p.getTextureCoords(x, y, !movingRight));
	};

	//head
	drawPart(head, 0, 0, skin.skinColor);
	drawPart(eyeWhite, 0, 0, Colors_White);
	drawPart(eye, 0, 0, skin.eyeColor);

	drawHair(skin.hairType, 0, 0, skin.hairColor); //hair
	
	drawPart(rightArm, 2, 2, skin.skinColor); //left arm
	
	if (skin.hasClothes)
	{
		drawPart(clothes, 0, 0, skin.clothesColor);
		drawPart(rightArm, 2, 0, skin.skinColor);
		drawPart(clothes, 0, 3, skin.clothesColor); // shoulder pad
	}
	else
	{
		drawPart(torso, 0, 0, skin.skinColor); 
		drawPart(rightArm, 2, 0, skin.skinColor);
	}
	
	if (skin.hasPants)
	{
		drawPart(pants, 0, 0, skin.pantsColor);
	}
	else
	{
		drawPart(legs, 0, 0, skin.skinColor);
	}



}
