#pragma once

#include "CommonLibrary.h"
#include "Map.h"
#include "Markers.h"
#include "Cards.h"
#include "Player.h"
#include "MapView.h"
#include "PlayerView.h"
#include "MedicObserver.h"
#include "ActionContext.h"
#include "Strategy.h"

class GameController
{
	int activePlayer;
	int numOfPlayers;
	bool eventCardInPlay;

	//Game Models
	Markers markers;
	GameMap* map;
	Deck* infectionDeck;
	Deck* playerDeck;
	vector<Player*> players;

	//Views
	MapView* mapDisplay; // = new MapView(&markers, map);
	PlayerView* playerDisplay;

	//Observer
	MedicObserver* medicObserver;

	//CheckBit
	bool quietNightPlayed;

public:
	// Constructor
	GameController();

	// Destructor
	~GameController();

	void initializeNewGame();
	void loadGame();
	void saveGame();

	void startRound();

private:
	//INITIALIZE HELPER FUNCTIONS///////////////////////////////////
	void initializePlayers();
	void addEpidemicCards();
	void initialInfection();
	void initializeObservers();
	
	void checkifEventCardinPlay();
	
	void switchPlayers();

	//PHASE 1 ACTIONS//////////////////////////////////////////////
	void phase1_Actions();

	//PHASE 2 DRAW CARDS////////////////////////////////////////////
	void phase2_DrawCards();

	//PHASE 3 INFECTION ////////////////////////////////////////////
	void phase3_Infects();

	//END GAME ///////////////////////////
	void endGame(bool isWon, string message);
};

