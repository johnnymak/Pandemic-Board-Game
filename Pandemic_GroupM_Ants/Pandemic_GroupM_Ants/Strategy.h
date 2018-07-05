#pragma once

#include "CommonLibrary.h"
#include "InputOutput.h"
#include "InfectCities.h"
#include "Map.h"
#include "Player.h"
#include "Markers.h"
#include "Command.h"

class Strategy
{
public:
	Strategy() {}
	virtual ~Strategy() {}

	virtual bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed) = 0;

//common helper function
protected:
	//Selects which players pawn will be moved. Handles special Dispatcher case
	void pawnSelection(vector<Player*> playerList, int activePlayer, int &movingPawn, bool isAirlift);
};


///////////////////////////////////////////////////////////////////////////////////////////////
// PHASE I ACTION CLASSES
//////////////////////////////////////////////////////////////////////////////////////////////

class DriveFerry : public Strategy
{
public:
	DriveFerry() {}
	~DriveFerry() {}

	//Player can move to a connecting city
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);
};

class DirectFlight : public Strategy
{
public:
	DirectFlight() {}
	~DirectFlight() {}

	//Player can discard a city card to fly directly to that city
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);
};

class CharterFlight : public Strategy
{
public:
	CharterFlight() {}
	~CharterFlight() {}

	//Player can discard a card matching their current city and move anywhere
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);
};

class ShuttleFlight : public Strategy
{
public:
	ShuttleFlight() {}
	~ShuttleFlight() {}

	//A Player in a city with a Research Station can move to any other city with a Research Station
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);
};

class DispatcherMovement : public Strategy
{
public:
	DispatcherMovement() {}
	~DispatcherMovement() {}

	//Dispatcher ONLY, can move any players pawn to the location of another players pawn.
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);
};

class BuildResearchStation : public Strategy
{
public:
	BuildResearchStation() {}
	~BuildResearchStation() {}

	//Player can build a research station in their current city if they discard the matching city card.
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);
};

class DiscoverCure : public Strategy
{
public:
	DiscoverCure() {}
	~DiscoverCure() {}

	//By Discarding 5 cards of the same color, the player can cure the disease of that color
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);
};

class TreatDisease : public Strategy
{
public:
	TreatDisease() {}
	~TreatDisease() {}

	//Player removes cubes of a disease from a city
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);
private:
	void checkifDiseaseNowEradicated(Markers* markers, InfectType infection);
};

class ShareKnowledge : public Strategy
{
public:
	ShareKnowledge() {}
	~ShareKnowledge() {}

	//Player can give other player a card if in the same city, and the card matches the city
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);

private:
	//Transfers the card from one players hand to another and checks player hand size
	void transferCard(int cardToPass, Player* fromPlayer, Player* toPlayer, Deck* playerDeck);
};

class ShowRefCard : public Strategy
{
public:
	ShowRefCard() {}
	~ShowRefCard() {}

	//Player views Reference Card description
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);

};

class TakeEventCard : public Strategy
{
public:
	TakeEventCard() {}
	~TakeEventCard() {}

	//Player views Reference Card description
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);

};

class DoNothing : public Strategy
{
public:
	DoNothing() {}
	~DoNothing() {}

	//Keeps program from crashing when player chooses not to act
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);

};

///////////////////////////////////////////////////////////////////////////////////////////////
// PHASE II ACTION CLASSES
//////////////////////////////////////////////////////////////////////////////////////////////

class DrawCard : public Strategy
{
public:
	DrawCard() {}
	~DrawCard() {}

	//Player draws a card and adds to hand or takes Epidemic card options
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);
private:
	//Handles events that occur when an Epidemic card is pulled
	void epidemicCardActions(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* infectionDeck);
};

///////////////////////////////////////////////////////////////////////////////////////////////
// PHASE III ACTION CLASSES
//////////////////////////////////////////////////////////////////////////////////////////////

class Infect : public Strategy
{
public:
	Infect() {}
	~Infect() {}

	//Keeps program from crashing when player chooses not to act
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);

};

///////////////////////////////////////////////////////////////////////////////////////////////
// EVENT CARDS
//////////////////////////////////////////////////////////////////////////////////////////////

// STRATEGY FOR EVENT CARDS
class DoEventCards : public Strategy
{
public:
	DoEventCards() {}
	~DoEventCards() {}

	//Player can move to a connecting city
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int playerID, bool &quietNightPlayed);
};
