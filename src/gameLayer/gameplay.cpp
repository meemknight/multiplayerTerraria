#include <gameplay.h>

float zoom = 30;
Player player;
Map map;

void initGameplay(PlayerSkin playerSkin)
{
	generateMap(map, 1234);

	map.renderMapIntoTexture();

	player = Player{};
	player.skin = playerSkin;
	zoom = 30;
	player.position.size = PLAYER_SIZE;
}

void runGameplay(float deltaTime, gl2d::Renderer2D &renderer, TileRenderer &tileRenderer,
	PlayerRenderer &playerRenderer)
{

	renderer.pushCamera();
	renderer.currentCamera.zoom = zoom;

#pragma region input

	const bool CREATIVE = 0;

	if (CREATIVE)
	{
		glm::vec2 direction = {};

		if (platform::isKeyHeld(platform::Button::A))
		{
			direction.x -= 1;
		}

		if (platform::isKeyHeld(platform::Button::D))
		{
			direction.x += 1;
		}

		if (platform::isKeyHeld(platform::Button::W))
		{
			direction.y -= 1;
		}

		if (platform::isKeyHeld(platform::Button::S))
		{
			direction.y += 1;
		}

		if (length(direction) != 0.f)
		{
			direction = normalize(direction);
		}

		player.position.position += deltaTime * direction * 20.f;
	}
	else
	{
		float direction = {};

		if (platform::isKeyHeld(platform::Button::A))
		{
			direction -= 1;
		}

		if (platform::isKeyHeld(platform::Button::D))
		{
			direction += 1;
		}

		player.moveVelocityX(10 * deltaTime * direction);

		if (platform::isKeyPressedOn(platform::Button::Space))
		{
			player.jump();
		}
	}



#pragma endregion

#pragma region player phisics

	if (!CREATIVE)
	{
		player.applyGravity(9.f);

		player.updatePhisics(deltaTime);

		player.resolveConstrains(map);
	}

	player.updateMove();

	player.playerAnimation.update(deltaTime);

#pragma endregion


	renderer.currentCamera.follow(player.position.getCenter(), 3, 0.0001, 0.2, platform::getWindowSizeX(), platform::getWindowSizeY());

	tileRenderer.renderMap(renderer, map);

	playerRenderer.render(renderer, player.position.position, player.skin,
		player.movingRight, player.playerAnimation);
	

#pragma region imgui
	ImGui::Begin("camera");

	ImGui::DragFloat("zoom", &zoom, 1, 1, 500);
	ImGui::DragFloat2("player pos", &player.position.position[0], 5);


	{
		auto s = ImGui::GetContentRegionMax();

		if (ImGui::BeginChild(6996, {}, false, ImGuiWindowFlags_HorizontalScrollbar))
		{

			float xsize = std::max((int)(s.x * 1) - 10, (int)(100 * 1));
			float aspect = (float)map.mapSize.x / map.mapSize.y;

			ImGui::Image((void *)map.texture, {2400,2400 / aspect},
				{0, 0}, {1, 1});

			ImGui::EndChild();
		}
	}


	ImGui::End();
#pragma endregion


	renderer.popCamera();

}