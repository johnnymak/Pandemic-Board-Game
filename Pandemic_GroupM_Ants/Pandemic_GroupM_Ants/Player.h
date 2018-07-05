#pragma once

#include "CommonLibrary.h"
#include "Map.h"
#include "Cards.h"
#include "InputOutput.h"

//PLAYER CLASS/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Player : public Subject
{
	//Attributes/////////////////////////////////
	int id;
	CityNode* pawn;
	vector<Card*> cardsInHand;
	ReferenceCard* reference;
	RoleCard* role;
	Card* extraPlannerCard;

	//Serialize//////////////////
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar.template register_type<Card>();
		ar.template register_type<CityNode>();
		ar & id & pawn & cardsInHand & reference & role;
	}

public:
	//Constructors////////////////////////////////
	Player();
	Player(int id, ReferenceCard* referenceCard);
	~Player();

	//Getters/////////////////////////////////
	CityNode* getPawn() { return this->pawn; }
	int getId() { return id; }
	RoleCard* getRole() { return this->role; }
	void setRole(RoleCard* role) { this->role = role; }
	ReferenceCard* getReferenceCard() { return this->reference; }
	int getNumOfCards() { return this->cardsInHand.size(); }
	Card* getCard(int index) { return this->cardsInHand[index]; }

	//Setters////////////////////////////
	void movePawn(CityNode* newLocation);
	Card** discardCard(int* index, int size);
	Card** addCard(Card* card);
	Card** moveAndDiscard(CityNode* newLocation, int index);
	void addExtraPlannerCard(Card* card);

	//Card Methods///////////////////////////////
	int checkifPlayerHasCard(string name);
	bool checkifPlayerHasCardAtIndex(string name, int index);
	bool checkifPlayerHasEventCard();
	void displayPlayerCardOptions();

	string printHand();

	//Operator overload//////////////////////////
	const bool operator==(const string name);

private:

	//Setters ////////////////////////////////////
	Card* useExtraPlannerCard();
	Card* removeCard(int index);
	Card** tooManyCards(); //Called to get rid of a card when Player has too many cards in hand
	
};

