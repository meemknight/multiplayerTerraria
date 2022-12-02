#include <client.h>
#include <enet/enet.h>
#include <packet.h>
#include <unordered_map>
#include <Ui.h>

#undef max
#undef min

float zoom = 30;
//Player player;
static Map map;

ENetHost *client;
ENetPeer *server;

int32_t cid;
std::unordered_map<int32_t, Player> players;

static Npc guide = {};

extern gl2d::Texture uiTexture;


void sendPlayerData(bool reliable)
{
	Packet p;
	p.cid = cid;
	p.header = headerUpdateConnection;
	sendPacket(server, p, (const char *)&players[cid], sizeof(Player), reliable, 0);
}


bool joinServer(PlayerSkin playerSkin, const char *ip)
{
	assert(!client);
		
	client = enet_host_create(nullptr, 1, 1, 0, 0);

	ENetAddress adress;
	ENetEvent event;

	if (ip[0] == 0)
	{
		enet_address_set_host(&adress, "127.0.0.1");
	}
	else
	{
		enet_address_set_host(&adress, ip);
	}

	adress.port = 77799;


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
			else if (p.header == headerPlaceBlock)
			{
				PlaceBlockPacket placeBlockPacket = *(PlaceBlockPacket *)data;
				map.unsafePlace(placeBlockPacket.posx, placeBlockPacket.posy, placeBlockPacket.type);
			}
			else if (p.header == headerUpdateNPC)
			{
				Npc npc = *(Npc*)data;
				
				float timer = guide.animationTimer;
				int frame = guide.frame;
				guide = npc;
				guide.animationTimer = timer;
				guide.frame = frame;
	
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


void placeBlockServer(int x, int y, unsigned short type)
{
	PlaceBlockPacket placeBlockPacket;
	placeBlockPacket.posx = x;
	placeBlockPacket.posy = y;
	placeBlockPacket.type = type;

	Packet p;
	p.cid = cid;
	p.header = headerPlaceBlock;
	sendPacket(server, p, (const char *)&placeBlockPacket, sizeof(placeBlockPacket), 1, 0); //todo channel 2
}

void runGameplay(float deltaTime, gl2d::Renderer2D &renderer, TileRenderer &tileRenderer,
	PlayerRenderer &playerRenderer, ItemRenderer &itemRenderer, NpcRenderer &npcRenderer)
{

	serverConnectionUpdate(client);

	renderer.pushCamera();
	renderer.currentCamera.zoom = zoom;

	auto &player = players[cid];
	bool playerChanged = false;

#pragma region input

	//creative is deprecated!!!!!!!!!!!!
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
		int input = 0;

		if (platform::isKeyHeld(platform::Button::A))
		{
			input -= 1;
		}

		if (platform::isKeyHeld(platform::Button::D))
		{
			input += 1;
		}

		if (player.input != input)
		{
			player.input = input;
			playerChanged = true;
		}


		if (platform::isKeyPressedOn(platform::Button::Space) && player.grounded)
		{
			player.jump();
			playerChanged = true;
		}
	}



#pragma endregion

#pragma region player phisics


	for (auto &i : players)
	{
		i.second.moveVelocityX(10 * deltaTime * i.second.input);

		if (!CREATIVE)
		{
			i.second.applyGravity(8.f);

			i.second.updatePhisics(deltaTime);

			i.second.grounded = false;
			i.second.resolveConstrains(map);
		}

		i.second.playerAnimation.grounded = i.second.grounded;

		i.second.updateMove();

		i.second.playerAnimation.update(deltaTime);
	}

	{
		guide.p.moveVelocityX(2 * deltaTime * guide.p.input);
		guide.p.applyGravity(8.f);
		guide.p.updatePhisics(deltaTime);
		guide.p.grounded = false;
		guide.p.resolveConstrains(map);
		guide.p.updateMove();

		if (guide.p.input > 0)
		{
			guide.p.movingRight = true;
		}
		else if (guide.p.input < 0)
		{
			guide.p.movingRight = false;
		}

		if (guide.p.playerAnimation.state == PlayerAnimation::running)
		{
			guide.animationTimer -= deltaTime;
			if (guide.animationTimer < 0.f)
			{
				guide.animationTimer = 0.12;

				if (guide.frame > 0)guide.frame--;

				guide.frame++;
				guide.frame %= 15;
				guide.frame++;

			}
		}
		else
		{
			guide.frame = 0;
			guide.animationTimer = 0.2;
		}
	}



#pragma endregion


	renderer.currentCamera.follow(player.position.getCenter(), 3, 0.0001, 0.2, platform::getWindowSizeX(), platform::getWindowSizeY());
	
	auto view = renderer.getViewRect();
	if (view.x < 0)
	{
		renderer.currentCamera.position.x -= view.x;
	}
	else if (view.x + view.z > map.mapSize.x)
	{
		renderer.currentCamera.position.x -= (view.x + view.z) - map.mapSize.x;
	}

	tileRenderer.renderMap(renderer, map);

	renderer.renderRectangle(gl2d::Rect{guide.p.position.position, glm::ivec2{38 / 16.f, 54 / 16.f}}, glm::vec2{}, 0.f, npcRenderer.texture.t,
		npcRenderer.texture.getTextureCoords(0, guide.frame, guide.p.movingRight));

	//playerRenderer.render(renderer, guide.p.position.position, player.skin,
	//	player.movingRight, player.playerAnimation);

	for (auto &i : players)
	{
		if(i.first != cid)
		playerRenderer.render(renderer, i.second.position.position, i.second.skin,
			i.second.movingRight, i.second.playerAnimation);
	}

	playerRenderer.render(renderer, player.position.position, player.skin,
		player.movingRight, player.playerAnimation);

#pragma region inventory

	Item inventory[5] = {1,2,3,4,0};
	static int currentItem = 0;

	if (platform::isKeyReleased(platform::Button::NR1))
	{
		currentItem = 0;
	}else
	if (platform::isKeyReleased(platform::Button::NR2))
	{
		currentItem = 1;
	}else
	if (platform::isKeyReleased(platform::Button::NR3))
	{
		currentItem = 2;
	}
	else
	if (platform::isKeyReleased(platform::Button::NR4))
	{
		currentItem = 3;
	}
	else
	if (platform::isKeyReleased(platform::Button::NR5))
	{
		currentItem = 4;
	};


#pragma endregion



#pragma region place blocks

	if (platform::isFocused())
	{
		glm::ivec2 mousePos = platform::getRelMousePosition();

		auto lerp = [](auto a, auto b, auto c)
		{
			return a *(1.f - c) + b * c;
		};

		glm::ivec2 blockPosition = lerp(glm::vec2(view.x, view.y),
			glm::vec2(view.x + view.z, view.y + view.w), glm::vec2(mousePos) / glm::vec2(platform::getWindowSize()));

		renderer.renderRectangle({blockPosition, 1, 1}, {0,0,1,0.4});


		if (platform::isLMousePressed() && blockPosition.x > 0 && blockPosition.y > 0
			&& blockPosition.x < map.mapSize.x - 1  && blockPosition.y < map.mapSize.y - 1)
		{
			auto item = inventory[currentItem];
			
			if (item.type == Item::pickaxe)
			{
				map.unsafePlace(blockPosition.x, blockPosition.y, 0);
				placeBlockServer(blockPosition.x, blockPosition.y, 0);
			}
			else if (item.type == Item::dirt)
			{
				map.unsafePlace(blockPosition.x, blockPosition.y, Tile::dirt);
				placeBlockServer(blockPosition.x, blockPosition.y, Tile::dirt);
			}
			else if (item.type == Item::stone)
			{
				map.unsafePlace(blockPosition.x, blockPosition.y, Tile::stone);
				placeBlockServer(blockPosition.x, blockPosition.y, Tile::stone);
			}
			else if (item.type == Item::woddenPlank)
			{
				map.unsafePlace(blockPosition.x, blockPosition.y, Tile::woddenPlank);
				placeBlockServer(blockPosition.x, blockPosition.y, Tile::woddenPlank);
			}

		}


	}



#pragma endregion


#pragma region UI
	{

		Ui::Frame frame({0, 0, platform::getWindowSize()});
		float padding = 10;

		auto inventoryBox = Ui::Box().xLeft(padding).xDimensionPercentage(0.1).yAspectRatio(1.f)();

		inventoryBox.z = std::min(inventoryBox.z, 70);
		inventoryBox.w = inventoryBox.z;

		renderer.pushCamera();

		for (int i = 0; i < 5; i++)
		{
			if (i == currentItem)
			{
				renderer.renderRectangle(inventoryBox, Colors_Black, {}, {}, uiTexture);
			}
			else
			{
				renderer.renderRectangle(inventoryBox, {}, {}, uiTexture);
			}
			
			if (inventory[i].type != 0)
			{
				glm::vec4 smaller = inventoryBox;
				smaller.z *= 0.7;
				smaller.w *= 0.7;

				smaller.x += (inventoryBox.z - smaller.z) / 2.f;
				smaller.y += (inventoryBox.w - smaller.w) / 2.f;

				renderer.renderRectangle(smaller, {}, {}, itemRenderer.textures[inventory[i].type]);
			}
			
			inventoryBox.x += padding + inventoryBox.z;
		}


		renderer.popCamera();
	}
#pragma endregion



#pragma region send player data

	{
		static float timer = 0;
		constexpr float updateTime = 1.f / 10;


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

