#pragma once
#include <stdint.h>
#include <enet/enet.h>

struct Packet
{
	int32_t header = 0;
	int32_t cid = 0;
	uint64_t counter = 0;
	char *getData()
	{
		return (char *)((&cid) + 1);
	}
};



enum
{
	headerNone = 0,
	headerReceiveCIDAndData,
	headerReceiveMapData,
	headerUpdateConnection, //update player position and other stuff
	
};

constexpr int SERVER_CHANNELS = 1;

void sendPacket(ENetPeer *to, Packet p, const char *data, size_t size, bool reliable, int channel);
char *parsePacket(ENetEvent &event, Packet &p, size_t &dataSize);