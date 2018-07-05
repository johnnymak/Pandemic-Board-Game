#include "ActionContext.h"

void ActionContext::setStrategy(Strategy* strategy) {
	if (this->strategy != nullptr) {
		delete this->strategy;
	}
	this->strategy = strategy;
}

bool ActionContext::execute(GameMap * map, Markers * markers, vector<Player*> playerList, Deck * playerDeck, Deck * infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	return strategy->execute(map, markers, playerList, playerDeck, infectionDeck, activePlayer, quietNightPlayed);
}
