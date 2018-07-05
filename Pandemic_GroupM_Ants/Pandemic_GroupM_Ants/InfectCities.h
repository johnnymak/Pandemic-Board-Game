#pragma once

#include "CommonLibrary.h"
#include "Map.h"
#include "Player.h"
#include "Markers.h"

class InfectCities
{
	static vector<string> outbreakHistory;

public:
	//Places a disease cube in a city
	void static infectCity(CityNode* city, InfectType infection, Markers* markers, vector<Player*> playerList, bool firstCall);

private: 
	//Disallow instances of this class
	InfectCities() {}
	~InfectCities() {}

	//Checks if city is next to a Quarentine Specialist or a Medic
	//Returns true, if city affected by Quarentine Specialist
	bool static quarantineCheck(CityNode* city, vector<Player*> playerList, bool isCured);

	//Handles infection of connected cities in the case of an outbreak
	void static handleOutbreak(CityNode * city, InfectType infection, Markers* markers, vector<Player*> playerList);
};

