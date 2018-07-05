#pragma once

#include "CommonLibrary.h"
#include "Map.h"
#include "Markers.h"
#include "Observer.h"
#include "Player.h"
#include <regex>

using namespace std;

#ifndef MAPVIEW_H
#define MAPVIEW_H

class MapView : public Observer {

	public:
		// Constructor
		MapView(Markers* markers, GameMap* map, vector<Player*> playerList);

		// Destructor
		~MapView();

		// Overriden Observer Function
		void Update(string message);

		// ============ MapView Methods ==============
		void updateMap(const vector<CityNode *> * cityList);  // Updates the map 
		void printMap();									  // Prints the map
		void clearScreen();									  // Clears the MapView Screen

		string MapView::fillNumber(int number);  // Local function to fill in blank space for consistency
		void setActivePlayer(int playerID);

	private:
		string cityInfection[NUM_OF_CITIES];	   // List of City Names
		string mapResearchStation[NUM_OF_CITIES];  // List of Research Station Locations
		string researchStationCities;			   // List of cities with research stations
		string mapPlayerPos[NUM_OF_CITIES]; 

		int activePlayer;

		// Subject to be attached
		Markers* subjMarker;
		GameMap* subjMap;
		vector<Player*> subjPlayers;
		
};

#endif