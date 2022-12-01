#include <playerRenderer.h>
#include <string>

BodyPart loadedBodyParts[PlayerRenderer::bodyParts::BODY_PARTS_COUNT] = {
	{0, {38, 54}},
	{1, {38, 54}},
	{2, {38, 54}},
	{3, {38, 54}}, //torso
	{7, {38, 54}}, //right arm
	{10, {38, 54}}, //feet arm


}
	;


void PlayerRenderer::loadAll()
{
	for (int i = 0; i < bodyParts::BODY_PARTS_COUNT; i++)
	{
		std::string f = RESOURCES_PATH "Player_0_" + std::to_string(loadedBodyParts[i].textureId) + ".png";


		loadedBodyParts[i].s.load(f.c_str(), loadedBodyParts[i].atlasSize.x, loadedBodyParts[i].atlasSize.y);
		

	}


}

void PlayerRenderer::render(gl2d::Renderer2D &renderer, glm::vec2 pos)
{

	float pixelSize = 1.f / 16;
	glm::vec2 pixelSize2{pixelSize, pixelSize};

	auto drawPart = [&](int i, int x, int y)
	{
		auto &p = loadedBodyParts[i];
		renderer.renderRectangle({pos, pixelSize2 * glm::vec2(p.atlasSize)}, {}, {},
			p.s.t, p.s.getTextureCoords(x, y));
	};

	//head
	for (int i = 0; i <= PlayerRenderer::bodyParts::eye; i++)
	{
		drawPart(i, 0, 0);
	}
	
	drawPart(rightArm, 2, 2);
	drawPart(torso, 0, 0);
	drawPart(rightArm, 2, 0);


	drawPart(legs, 0, 0);

		


}
