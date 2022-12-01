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

void PlayerRenderer::render(gl2d::Renderer2D &renderer, glm::vec2 pos, PlayerSkin &skin, bool movingRight
	, PlayerAnimation &animation, float size)
{

	float pixelSize = 1.f / 16;
	glm::vec2 pixelSize2{pixelSize, pixelSize};

	pos.x -= pixelSize * 8;
	pos.y -= pixelSize * 4;


	int headFrame = animation.headFrame;
	int hairFrame = animation.hairFrame;

	auto drawPart = [&](int i, int x, int y, glm::vec3 color)
	{
		auto &p = loadedBodyParts[i];
		renderer.renderRectangle({pos, size * pixelSize2 * glm::vec2(p.atlasSize)},
			glm::vec4(color, 1.f), {}, {},
			p.s.t, p.s.getTextureCoords(x, y, !movingRight));
	};

	auto drawHair = [&](int i, int x, int y, glm::vec3 color)
	{
		auto &p = hairSprites[i];
		renderer.renderRectangle({pos, size * pixelSize2 * glm::vec2(p.blockSize)},
			glm::vec4(color, 1.f), {}, {},
			p.t, p.getTextureCoords(x, y, !movingRight));
	};

	//head
	drawPart(head, 0, headFrame, skin.skinColor);
	drawPart(eyeWhite, 0, headFrame, Colors_White);
	drawPart(eye, 0, headFrame, skin.eyeColor);

	drawHair(skin.hairType, 0, hairFrame, skin.hairColor); //hair
	
	if (animation.isFrameUp)
	{
		pos.y -= pixelSize * 2 * size;
	}

	drawPart(rightArm, animation.handFrameX, 2, skin.skinColor); //left arm
	
	if (skin.hasClothes)
	{
		drawPart(clothes, 0, 0, skin.clothesColor);
		drawPart(rightArm, animation.handFrameX, animation.handFrameY, skin.skinColor);
		drawPart(clothes, 0, 3, skin.clothesColor); // shoulder pad
	}
	else
	{
		drawPart(torso, 0, 0, skin.skinColor); 
		drawPart(rightArm, animation.handFrameX, animation.handFrameY, skin.skinColor);
	}

	if (animation.isFrameUp)
	{
		pos.y += pixelSize * 2 * size;
	}
	
	if (skin.hasPants)
	{
		drawPart(pants, 0, headFrame, skin.pantsColor);
	}
	else
	{
		drawPart(legs, 0, headFrame, skin.skinColor);
	}



}

void PlayerAnimation::update(float deltaTime)
{
	if (state == stay)
	{
		*this = PlayerAnimation{};
	}
	else if (state == running)
	{
		float runSpeed = 0.08;

		timer += deltaTime;

		if (headFrame >= 6)
		{
			headFrame -= 6;
		}

		while (timer >= runSpeed)
		{
			timer -= runSpeed;
			headFrame++;
		}

		headFrame %= 14;
				
		if ((headFrame >= 1 && headFrame <= 3) || (headFrame >= 8 && headFrame <= 10))
		{ isFrameUp = true; }else{ isFrameUp = false; }

		hairFrame = headFrame;

		handFrameX = (headFrame/2) % 4 + 3;
		handFrameY = 1;

		headFrame += 6;


	}


}
