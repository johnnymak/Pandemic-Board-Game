#pragma once

#include "Observer.h"
#include "Player.h"
#include "Markers.h"

class MedicObserver : public Observer
{
	Player* player;
	GameMap* map;
	Markers* markers;

public:
	MedicObserver() {}
	MedicObserver(Player* player, GameMap* map, Markers* markers);
	~MedicObserver() {}

	// void Update();
	void Update(string message);
};

