#pragma once

#include "CommonLibrary.h"
#include "Strategy.h"

class ActionContext
{
	Strategy* strategy;

public:
	ActionContext() {};
	ActionContext(Strategy* strategy) : strategy(strategy) {}
	~ActionContext() { delete strategy; strategy = nullptr; }

	void setStrategy(Strategy* strategy);
	bool execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed);
};

