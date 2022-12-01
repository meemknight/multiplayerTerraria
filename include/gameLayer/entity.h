#pragma once

#include <glm/glm.hpp>
#include <playerRenderer.h>
#include <map.h>

struct Transform
{

	glm::vec2 position = {};
	glm::vec2 size = {};

	glm::vec2 getTopLeftCorner();
	glm::vec2 getCenter();
	glm::vec2 getBottomLeftCorner();
	glm::vec2 getBottomCenter();

};

#define PLAYER_SIZE glm::vec2(22.f/16.f,46.f/ 16.f)

struct Player
{
	Transform position = {glm::vec2(0,0), PLAYER_SIZE};

	glm::vec2 lastPos{};

	bool movingRight = 0;

	void move(glm::vec2 dir);

	//should be called only once per frame
	void updateMove();

	void resolveConstrains(Map &mapData);

	void checkCollisionBrute(glm::vec2 &pos, glm::vec2 lastPos,
		Map &mapData, bool &upTouch, bool &downTouch, bool &leftTouch, bool &rightTouch);

	glm::vec2 Player::performCollision(Map &mapData, glm::vec2 pos, glm::vec2 size,
		glm::vec2 delta, bool &upTouch, bool &downTouch, bool &leftTouch, bool &rightTouch);


	PlayerSkin skin = {};


};