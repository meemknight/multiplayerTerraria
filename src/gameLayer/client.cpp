#include <client.h>
#include <enet/enet.h>
#include <packet.h>
#include <unordered_map>


#undef max
#undef min

float zoom = 30;
//Player player;
static Map map;

ENetHost *client;
ENetPeer *server;

int32_t cid;
std::unordered_map<int32_t, Player> players;


void sendPlayerData(bool reliable)
{
	Packet p;
	p.cid = cid;
	p.header = headerUpdateConnection;
	sendPacket(server, p, (const char *)&players[cid], sizeof(Player), reliable, 0);
}


bool joinServer(PlayerSkin playerSkin)
{
	assert(!client);
		
	client = enet_host_create(nullptr, 1, 1, 0, 0);

	ENetAddress adress;
	ENetEvent event;
	enet_address_set_host(&adress, "127.0.0.1");
	adress.port = 7779;


	server = enet_host_connect(client, &adress, SERVER_CHANNELS, 0);

	if (server == nullptr)
	{
		enet_host_destroy(client);
		client = 0;

		return false;
	}

	if (enet_host_service(client, &event, 5000) > 0
		&& event.type == ENET_EVENT_TYPE_CONNECT)
	{
		//std::cout << "connected\n";
	}
	else
	{
		//we first should recieve a connect message, if not we couldn't connect
		enet_peer_reset(server);
		enet_host_destroy(client);
		client = 0;
		server = 0;
		
		return false;
	}

	if (enet_host_service(client, &event, 5000) > 0
		&& event.type == ENET_EVENT_TYPE_RECEIVE)
	{
		Packet p = {};
		size_t size;
		auto data = parsePacket(event, p, size);

		if (p.header != headerReceiveCIDAndData)
		{
			enet_peer_reset(server);
			enet_host_destroy(client);
			client = 0;
			server = 0;
			return false;
		}

		cid = p.cid;

		glm::ivec2 mapSize = *(glm::ivec2*)data;
		
		map.create(mapSize.x, mapSize.y);

		//std::cout << "received cid: " << cid << "\n";
		enet_packet_destroy(event.packet);


	}
	else
	{
		enet_peer_reset(server);
		enet_host_destroy(client);
		client = 0;
		server = 0;

		return false;
	}

	if (enet_host_service(client, &event, 5000) > 0
		&& event.type == ENET_EVENT_TYPE_RECEIVE)
	{
		Packet p = {};
		size_t size;
		auto data = parsePacket(event, p, size);

		if (p.header != headerReceiveMapData)
		{
			enet_peer_reset(server);
			enet_host_destroy(client);
			client = 0;
			server = 0;
			return false;
		}

		Tile *mapTiles = (Tile *)data;

		std::memcpy(&map.tiles[0], mapTiles, sizeof(Tile) * map.mapSize.x * map.mapSize.y);

		map.renderMapIntoTexture();

		enet_packet_destroy(event.packet);

	}
	else
	{
		enet_peer_reset(server);
		enet_host_destroy(client);
		client = 0;
		server = 0;

		return false;
	}

	//then we recieve player position
	if (enet_host_service(client, &event, 5000) > 0
		&& event.type == ENET_EVENT_TYPE_RECEIVE)
	{
		Packet p = {};
		size_t size = {};
		auto data = parsePacket(event, p, size);

		if (p.header == headerUpdateConnection)
		{

			//players[p.cid] = *(phisics::Entity *)data;
			players[p.cid] = *(Player *)data;
			players[p.cid].skin = playerSkin;
			
			//send skin
			sendPlayerData(true);

		}
		else
		{
			enet_peer_reset(server);
			enet_host_destroy(client);
			client = 0;
			server = 0;
			return false;
		}

		enet_packet_destroy(event.packet);

	}
	else
	{
		enet_peer_reset(server);
		enet_host_destroy(client);
		client = 0;
		server = 0;

		return false;
	}

	return true;

}


void serverConnectionUpdate(ENetHost *client)
{
	ENetEvent event;
	if (enet_host_service(client, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
		{

			Packet p = {};
			size_t size = {};
			auto data = parsePacket(event, p, size);

			if (p.header == headerUpdateConnection)
			{

				players[p.cid] = *(Player *)data;
				//player = *(Player *)data;

			}
			
		
			enet_packet_destroy(event.packet);
			break;
		}
		
		case ENET_EVENT_TYPE_DISCONNECT:
		{
			//std::cout << "disconect\n";
			exit(0);

		
			break;
		}

		}
	}

}


void runGameplay(float deltaTime, gl2d::Renderer2D &renderer, TileRenderer &tileRenderer,
	PlayerRenderer &playerRenderer)
{

	serverConnectionUpdate(client);

	renderer.pushCamera();
	renderer.currentCamera.zoom = zoom;

	auto &player = players[cid];

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


	for (auto &i : players)
	{
		if (!CREATIVE)
		{
			i.second.applyGravity(9.f);

			i.second.updatePhisics(deltaTime);

			i.second.resolveConstrains(map);
		}

		i.second.updateMove();

		i.second.playerAnimation.update(deltaTime);
	}



#pragma endregion


	renderer.currentCamera.follow(player.position.getCenter(), 3, 0.0001, 0.2, platform::getWindowSizeX(), platform::getWindowSizeY());

	tileRenderer.renderMap(renderer, map);

	for (auto &i : players)
	{
		if(i.first != cid)
		playerRenderer.render(renderer, i.second.position.position, i.second.skin,
			i.second.movingRight, i.second.playerAnimation);
	}

	playerRenderer.render(renderer, player.position.position, player.skin,
		player.movingRight, player.playerAnimation);


#pragma region send player data

	{
		static float timer = 0;
		constexpr float updateTime = 1.f / 10;
		bool playerChanged = false;


		timer -= deltaTime;
		if (playerChanged || timer <= 0)
		{
			timer = updateTime;
			playerChanged = true;
		}

		if (playerChanged)
		{
			sendPlayerData(false);
		}
	}

#pragma endregion



#pragma region imgui
	/*
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
	*/
#pragma endregion


	renderer.popCamera();

}

