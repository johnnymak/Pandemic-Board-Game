#ifndef CARDFACTORY_H
#define CARDFACTORY_H

#include "CommonLibrary.h"
#include "Cards.h"

class Card;
class CityCard;
class EventCard;
class InfectionCard;
class RoleCard;

class CardFactory
{
public:
	static Card* CreateCard(ifstream& file, string type);
private:
	CardFactory() {};
};

#endif