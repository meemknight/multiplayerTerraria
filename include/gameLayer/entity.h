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

	glm::vec2 velocity = {};

	bool movingRight = 0;
	bool grounded = 0;

	void move(glm::vec2 dir);

	void moveVelocityX(float dir);

	void jump();

	void applyGravity(float gravity);

	bool movingThisFrame = false;

	//should be called only once per frame last
	void updateMove();

	//this should be called before collisions 
	void updatePhisics(float deltaTime);


	void resolveConstrains(Map &mapData);

	void checkCollisionBrute(glm::vec2 &pos, glm::vec2 lastPos,
		Map &mapData, bool &upTouch, bool &downTouch, bool &leftTouch, bool &rightTouch);

	glm::vec2 Player::performCollision(Map &mapData, glm::vec2 pos, glm::vec2 size,
		glm::vec2 delta, bool &upTouch, bool &downTouch, bool &leftTouch, bool &rightTouch);


	PlayerSkin skin = {};


};