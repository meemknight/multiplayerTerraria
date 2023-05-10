#include <server.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>
#include <npcs.h>

std::atomic_bool serverIsRunning = false;
std::atomic_bool shouldCloseServer = true;

static Map map;

static Npc guide = {};

std::unordered_map<int32_t, Client> connections;

void broadCast(Packet p, void *data, size_t size, ENetPeer *peerToIgnore, bool reliable, int channel)
{
	for (auto it = connections.begin(); it != connections.end(); it++)
	{
		if (!peerToIgnore || (it->second.peer != peerToIgnore))
		{
			sendPacket(it->second.peer, p, (const char *)data, size, true, channel);
		}
	}
}

void launchServer()
{
	if (serverIsRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	assert(!serverIsRunning);

	shouldCloseServer = false;
	serverIsRunning = true;

	generateMap(map, 1234);

	guide.p.position.position.x = map.mapSize.x/2;


	std::thread t(serverFunction);
	t.detach();

}

void closeServer()
{
	shouldCloseServer = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); //wait for the server thread to have some more time to close

}

int pids = 1;
bool changedData = 0;

void sendNpcData(ENetPeer *to, bool reliable)
{
	Packet p;
	p.header = headerUpdateNPC;
	p.cid = 0; //server
	sendPacket(to, p, (const char *)&guide, sizeof(guide), reliable, 0);
}

void broadCastNpcData(bool reliable)
{
	for (auto it = connections.begin(); it != connections.end(); it++)
	{
		sendNpcData(it->second.peer, reliable);
	}
}

void sendMapData(ENetPeer *to)
{
	Packet p;
	p.header = headerReceiveMapData;
	p.cid = 0; //server

	sendPacket(to, p, (const char *)&map.tiles[0], sizeof(map.tiles[0]) * map.tiles.size(), true, 0);
}

void addConnection(ENetHost *server, ENetEvent &event)
{
	changedData = true;

	{
		Packet p;
		p.header = headerReceiveCIDAndData;
		p.cid = pids;

		//send own cid and map size
		sendPacket(event.peer, p, (const char *)&map.mapSize, sizeof(map.mapSize), true, 0);
	}


	connections[pids].playerData = Player{};
	connections[pids].playerData.position.position.x = map.mapSize.x/2;
	connections[pids].peer = event.peer;

	//send map
	sendMapData(event.peer);

	//send player data to this player
	{
		Packet p;
		p.header = headerUpdateConnection;
		p.cid = pids;
		sendPacket(event.peer, p, (const char *)&connections[pids].playerData,
			sizeof(Player), true, 0);
	
	}

	//send this player all the other info
	for (auto &i : connections)
	{
		if (i.first != pids)
		{
			Packet p;
			p.header = headerUpdateConnection;
			p.cid = i.first;
			sendPacket(event.peer, p, (const char *)&i.second.playerData,
				sizeof(Player), true, 0);
		}
	}


	//send npc data;
	sendNpcData(event.peer, true);

	pids++;

}

void recieveData(ENetHost *server, ENetEvent &event)
{
	changedData = true;

	Packet p;
	size_t size = 0;
	auto data = parsePacket(event, p, size);

	//validate data
	auto it = connections.find(p.cid);
	if (it != connections.end() && it->second.peer != event.peer)
	{
		//std::cout << "invalid data!\n";
		return;
	}

	if (p.header == headerUpdateConnection)
	{
		it->second.playerData = *(Player*)(data);
		it->second.changed = true;
		
		if (!it->second.ready)
		{
			it->second.ready = true;
				//first time, send to others so they have its skin and position
			std::cout << "recieved a player";

			broadCast(p, (void*)&it->second.playerData, sizeof(Player), event.peer, true, 0);
		}
		
		
	}
	else if (p.header == headerPlaceBlock)
	{
		PlaceBlockPacket placeBlockPacket = *(PlaceBlockPacket *)data;
		map.unsafePlace(placeBlockPacket.posx, placeBlockPacket.posy, placeBlockPacket.type);

		broadCast(p, (void *)data, sizeof(PlaceBlockPacket), event.peer, true, 0); //todo channel 2
	}

	enet_packet_destroy(event.packet);


}

void serverFunction()
{
	serverIsRunning = true;

	ENetAddress adress;
	adress.host = ENET_HOST_ANY;
	adress.port = 77799;
	ENetEvent event = {};

	//first param adress, players limit, channels, bandwith limit
	ENetHost *server = enet_host_create(&adress, 32, SERVER_CHANNELS, 0, 0);

	if (!server)
	{
		std::terminate();
	}

	while (!shouldCloseServer)
	{

		int counter = 0;
		constexpr int maxCounter = 10; //max packets to recieve per frame

		//recieve some data from players
		while (enet_host_service(server, &event, 0) > 0 && counter < maxCounter && !shouldCloseServer)
		{
			counter++;

			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				addConnection(server, event);

				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
			{
				recieveData(server, event);

				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				//std::cout << "disconnect: "
				//	<< event.peer->address.host << " "
				//	<< event.peer->address.port << "\n\n";
				//removeConnection(server, event);
				break;
			}
			}


		}

		//broadcast some changes
		if (!shouldCloseServer)
		{

			if (changedData)
			{
				for (auto p = connections.begin(); p != connections.end(); p++)
				{

					if (!p->second.changed)
					{
						continue;
					}

					p->second.changed = false;

					Packet sPacket;
					sPacket.header = headerUpdateConnection;
					sPacket.cid = p->first;
					broadCast(sPacket, &p->second.playerData, sizeof(Player), p->second.peer, false, 0);
				}

				changedData = false;
			}



		}

		//simulate the game
		if (!shouldCloseServer)
		{

			float deltaTime = 0.f;
			{
				static auto stop = std::chrono::high_resolution_clock::now();
				auto start = std::chrono::high_resolution_clock::now();

				deltaTime = (std::chrono::duration_cast<std::chrono::microseconds>(start - stop)).count() / 1'000'000.0f;
				stop = std::chrono::high_resolution_clock::now();

				//std::cout << deltaTime << "\n";
			}


			//npc
			bool npcChanged = false;

			{
				static std::mt19937 rng{std::random_device()()};

				static float changeTimer = 1;

				static int dir = 0;

				changeTimer -= deltaTime;

				if (changeTimer < 0.f)
				{
					std::uniform_real_distribution<float> dist(1, 5);
					changeTimer = dist(rng);

					std::uniform_int_distribution<int> distDir(-1, 1);
					dir = distDir(rng);
				}

				if (guide.p.input != dir)
				{
					guide.p.input = dir;
					npcChanged = true;
				}

				guide.p.moveVelocityX(2 * deltaTime * guide.p.input);
				guide.p.applyGravity(8.f);
				guide.p.updatePhisics(deltaTime);
				guide.p.grounded = false;
				guide.p.resolveConstrains(map);
				guide.p.updateMove();


			}

			//send npc data
			{
				static float timer = 0;
				constexpr float updateTime = 1.f;

				timer -= deltaTime;
				if (npcChanged || timer <= 0)
				{
					timer = updateTime;
					npcChanged = true;
				}

				if (npcChanged)
				{
					broadCastNpcData(false);
				}
			}



		}

	}


	serverIsRunning = false;
}
