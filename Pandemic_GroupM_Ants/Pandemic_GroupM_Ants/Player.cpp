#include "Player.h"

//PLAYER CLASS///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Constructors////////////////////////////////////////////
Player::Player() : id(0), pawn(nullptr), role(nullptr), reference(nullptr){}

Player::Player(int id, ReferenceCard* referenceCard) : id(id), reference(referenceCard) {}

Player::~Player()
{
	for (int i = 0; i < cardsInHand.size(); i++) {
		delete this->cardsInHand[i];
		this->cardsInHand[i] = nullptr;
	}
	delete reference;
	reference = nullptr;
	delete role;
	role = nullptr;
}

void Player::movePawn(CityNode * newLocation)
{
	this->pawn = newLocation;
	Notify("Player " + to_string(id) + " moved");
}

Card** Player::discardCard(int* index, int size)
{
	Card** cards = new Card*[size];
	int indexBase = cardsInHand.size();
	for (int i = 0; i < size; i++) {
		if (*index < indexBase) {
			cards[i] = this->removeCard(*index);
		}
		else {
			Card* card = useExtraPlannerCard();
		}
		index++;
	}
	Notify("Player " + to_string(id) + " discarded card");
	return cards;
}

Card** Player::addCard(Card * card)
{
	this->cardsInHand.push_back(card);
	if (cardsInHand.size() > MAX_HAND_SIZE) {
		return tooManyCards();
	}
	else {
		Notify("Player " + to_string(id) + " added card");
		return nullptr;
	}
}

Card** Player::moveAndDiscard(CityNode * newLocation, int index)
{
	this->pawn = newLocation;
	Card* card;
	if (index < cardsInHand.size()) {
		card = this->removeCard(index);
	}
	else {
		card = useExtraPlannerCard();
	}
	Notify("Player " + to_string(id) + " moved");
	return &card;
}

void Player::addExtraPlannerCard(Card* card)
{
	extraPlannerCard = card;
	Notify("Player " + to_string(id) + " added card");
}

//Card Functions////////////////////////////////////////////
//checks if player is holding a specific card and if true, returns the index position of the card in the players hand
int Player::checkifPlayerHasCard(string name)
{
	int cardIndex = -1;
	for (int i = 0; i < cardsInHand.size(); i++)
	{
 		if (*cardsInHand[i] == name)
		{
			cardIndex = i;
			return cardIndex;
		}
	}
	return cardIndex;
}

bool Player::checkifPlayerHasCardAtIndex(string name, int index) {
	return *cardsInHand[index] == name;
}

bool Player::checkifPlayerHasEventCard()
{
	for (int i = 0; i < cardsInHand.size(); i++)
	{
		if (cardsInHand[i]->getType() == "Event")
		{
			return true;
		}
	}
	return false;
}

//Prints cards in hand, and key selections to select from them.
void Player::displayPlayerCardOptions()
{
	for (int i = 0; i < cardsInHand.size(); i++) {
		cout << i << ") " << cardsInHand[i]->print() << endl;
	}
}

Card** Player::tooManyCards()
{
	cout << "\nPlayer " << id+1 << " has too many cards in hand! Select one to discard:" << endl;
	this->displayPlayerCardOptions();
	int answer;

	// Input Tick for visual purposes 
	cout << "> ";

	// Check input
	while (!getIntInput(answer, 0, cardsInHand.size())) {}

	// Discard Card
	cout << "Discarding Card\n" << endl;
	 return discardCard(&answer, 1);
}

string Player::printHand() {
	string hand = "Hand: \n";
	for (int i = 0; i < cardsInHand.size(); i++) {
		hand.append("      - " + cardsInHand.at(i)->print() + "\n");
	}
	return hand;
}

const bool Player::operator==(const string name)
{
	if (*role == name) {
		return true;
	}
	else {
		return false;
	}
}

//Private Functions///////////////////////////////////////////////////////

Card* Player::useExtraPlannerCard()
{
	Card* card = extraPlannerCard;
	extraPlannerCard = nullptr;
	return card;
}

Card* Player::removeCard(int index)
{
	if (index < cardsInHand.size() && index > -1) {
		Card* temp = this->cardsInHand[index];
		this->cardsInHand.erase(cardsInHand.begin() + index);
		return temp;
	}
	else
		return nullptr;
}
