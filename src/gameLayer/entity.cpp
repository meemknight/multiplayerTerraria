#include <entity.h>
#include <phisics.h>

glm::vec2 Transform::getTopLeftCorner()
{
	return position;
}

glm::vec2 Transform::getCenter()
{
	return position + size / 2.f;
}

glm::vec2 Transform::getBottomLeftCorner()
{
	return position + glm::vec2(0, size.y);
}

glm::vec2 Transform::getBottomCenter()
{
	return position + glm::vec2(size.x/2.f, size.y);
}

void Player::move(glm::vec2 dir)
{
	position.position += dir;
}

void Player::updateMove()
{
	if (lastPos.x - position.position.x < 0)
	{
		movingRight = true;
	}
	else if (lastPos.x - position.position.x > 0)
	{
		movingRight = false;
	}

	lastPos = position.position;
}

void Player::checkCollisionBrute(glm::vec2 &pos, glm::vec2 lastPos, Map &mapData, bool &upTouch, bool &downTouch, bool &leftTouch, bool &rightTouch)
{
	glm::vec2 delta = pos - lastPos;
	const float BLOCK_SIZE = 1.f;

	glm::vec2 &dimensions = position.size;

	if (
		(pos.y < -dimensions.y)
		|| (pos.x < -dimensions.x)
		|| (pos.y > mapData.mapSize.x * BLOCK_SIZE)
		|| (pos.x > mapData.mapSize.y * BLOCK_SIZE)
		)
	{
		return;
	}

	glm::vec2 newPos = performCollision(mapData, {pos.x, lastPos.y}, {dimensions.x, dimensions.y}, {delta.x, 0},
		upTouch, downTouch, leftTouch, rightTouch);
	pos = performCollision(mapData, {newPos.x, pos.y}, {dimensions.x, dimensions.y}, {0, delta.y},
		upTouch, downTouch, leftTouch, rightTouch);

}

glm::vec2 Player::performCollision(Map &mapData, glm::vec2 pos, glm::vec2 size, glm::vec2 delta, bool &upTouch, bool &downTouch, bool &leftTouch, bool &rightTouch)
{
	int minX = 0;
	int minY = 0;
	int maxX = mapData.mapSize.x;
	int maxY = mapData.mapSize.y;

	auto &dimensions = position.size;

	const float BLOCK_SIZE = 1.f;

	minX = (pos.x - abs(delta.x) - BLOCK_SIZE) / BLOCK_SIZE;
	maxX = ceil((pos.x + abs(delta.x) + BLOCK_SIZE + size.x) / BLOCK_SIZE);

	minY = (pos.y - abs(delta.y) - BLOCK_SIZE) / BLOCK_SIZE;
	maxY = ceil((pos.y + abs(delta.y) + BLOCK_SIZE + size.y) / BLOCK_SIZE);

	minX = std::max(0, minX);
	minY = std::max(0, minY);
	maxX = std::min(mapData.mapSize.x, maxX);
	maxY = std::min(mapData.mapSize.y, maxY);

	for (int y = minY; y < maxY; y++)
		for (int x = minX; x < maxX; x++)
		{
			if (mapData.unsafeGet(x, y).isCollidable())
			{
				if (aabb({pos,dimensions}, {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE}, 0.0001f))
				{
					if (delta.x != 0)
					{
						if (delta.x < 0) // moving left
						{
							leftTouch = 1;
							pos.x = x * BLOCK_SIZE + BLOCK_SIZE;
							goto end;
						}
						else
						{
							rightTouch = 1;
							pos.x = x * BLOCK_SIZE - dimensions.x;
							goto end;
						}
					}
					else if (delta.y != 0)
					{
						if (delta.y < 0) //moving up
						{
							upTouch = 1;
							pos.y = y * BLOCK_SIZE + BLOCK_SIZE;
							goto end;
						}
						else
						{
							downTouch = 1;
							pos.y = y * BLOCK_SIZE - dimensions.y;
							goto end;
						}
					}

				}
			}

		}

end:
	return pos;

}

void Player::resolveConstrains(Map &mapData)
{

	bool upTouch = 0;
	bool downTouch = 0;
	bool leftTouch = 0;
	bool rightTouch = 0;

	glm::vec2 &pos = position.position;

	float distance = glm::length(lastPos - pos);
	const float BLOCK_SIZE = 1.f;

	if (distance < BLOCK_SIZE)
	{
		checkCollisionBrute(pos,
			lastPos,
			mapData,
			upTouch,
			downTouch,
			leftTouch,
			rightTouch
		);
	}
	else
	{
		glm::vec2 newPos = lastPos;
		glm::vec2 delta = pos - lastPos;
		delta = glm::normalize(delta);
		delta *= 0.9 * BLOCK_SIZE;

		do
		{
			newPos += delta;
			glm::vec2 posTest = newPos;
			checkCollisionBrute(newPos,
				lastPos,
				mapData,
				upTouch,
				downTouch,
				leftTouch,
				rightTouch);

			if (newPos != posTest)
			{
				pos = newPos;
				goto end;
			}

		} while (glm::length((newPos + delta) - pos) > 1.0f * BLOCK_SIZE);

		checkCollisionBrute(pos,
			lastPos,
			mapData,
			upTouch,
			downTouch,
			leftTouch,
			rightTouch);
	}

end:

	//clamp the box if needed
	if (pos.x < 0) { pos.x = 0; }
	if (pos.x + position.size.x > (mapData.mapSize.x) * BLOCK_SIZE) 
	{ pos.x = ((mapData.mapSize.y) * BLOCK_SIZE) - position.size.x; }
	void;

}
