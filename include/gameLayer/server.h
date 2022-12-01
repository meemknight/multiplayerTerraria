#pragma once

#include <enet/enet.h>
#include <entity.h>
#include <unordered_map>
#include <packet.h>

struct Client
{
	ENetPeer *peer = {}; //connection to the player
	Player playerData = {};
	bool changed = 1; //used by the server
	bool ready = false;
};



void broadCast(Packet p, void *data, size_t size, ENetPeer *peerToIgnore, bool reliable, int channel);

void launchServer();
void closeServer();

void serverFunction();

void addConnection(ENetHost *server, ENetEvent &event);


