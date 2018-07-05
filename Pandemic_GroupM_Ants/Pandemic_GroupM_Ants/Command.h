#pragma once
#include "CommonLibrary.h"
#include "Map.h"
#include "Player.h"

class Command
{
public:
	Command() {}
	virtual ~Command() {}

	virtual void execute() = 0;
};

class MovePawn : public Command
{
	Player* player;
	CityNode* newLocation;

public:
	MovePawn() {}
	MovePawn(Player* player, CityNode* newLocation) : player(player), newLocation(newLocation) {}
	virtual ~MovePawn() {}

	virtual void execute() { player->movePawn(newLocation); }
};

class DiscardCard : public Command
{
	Player* player;
	int* index;
	int size;
	Deck* playerDeck;
public:
	DiscardCard() {}
	DiscardCard(Player* player, int* index, int size, Deck* playerDeck) : player(player), index(index), size(size), playerDeck(playerDeck) {}
	virtual ~DiscardCard() {}

	virtual void execute();
};

class DiscardCardAndMove : public Command
{
	Player* player;
	CityNode* newLocation;
	int index;
	Deck* playerDeck;
public:
	DiscardCardAndMove() {}
	DiscardCardAndMove(Player* player, CityNode* newLocation, int index, Deck* playerDeck) : player(player), newLocation(newLocation), index(index), playerDeck(playerDeck) {}
	virtual ~DiscardCardAndMove() {}

	virtual void execute();
};

class AddCardToHand : public Command
{
	Player* player;
	Card* card;
	Deck* playerDeck;

public:
	AddCardToHand() {}
	AddCardToHand(Player* player, Card* card, Deck* playerDeck) : player(player), card(card), playerDeck(playerDeck) {}
	virtual ~AddCardToHand() {}

	virtual void execute();
};

class TransferCard : public Command
{
	Player* fromPlayer;
	Player* toPlayer;
	int index;
	Deck* playerDeck;

public:
	TransferCard() {}
	TransferCard(Player* fromPlayer, Player* toPlayer, int index, Deck* playerDeck) : fromPlayer(fromPlayer), toPlayer(toPlayer), index(index), playerDeck(playerDeck) {}
	virtual ~TransferCard() {}

	virtual void execute();
};

class addCardToPocket: public Command
{
	Player* player;
	Card* card;

public:
	addCardToPocket() {}
	addCardToPocket(Player* player, Card* card) : player(player), card(card) {}
	virtual ~addCardToPocket() {}

	virtual void execute();
};