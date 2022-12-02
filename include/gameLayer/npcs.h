#pragma once
#include <sprites.h>
#include <entity.h>

struct Npc
{
	//todo somethig else
	Player p;

	float animationTimer = 0;
	int frame = 0;


};


struct NpcRenderer
{

	Sprite texture;

	void load();

};
