#pragma once

#include "CommonLibrary.h"
#include "CardFactory.h"
#include <algorithm>
#include <ctime>
#include <deque>

/*
* CARD CLASS
* - Used to create "physical" Card objects that the Deck will hold.
* - This is a Base class, individual types of cards will be created in its Derived classes.
*/

class Card {
private:
	string type; // The type of card - Role, Reference, Infection, or Player

	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & type;
	}

public:
	Card(); // Default constructor
	Card(string type); // Constructor that takes in a Type
	virtual ~Card(); // Default destructor (virtual)
	void setType(string type); // Sets the type of the Card
	string getType() const; // Returns the type of the Card object
	virtual string print() { return  ""; } // Virtual function that will be used to print out the Card object in its derived classes

	//Operator overload//
	virtual const bool operator==(const string str) const {	return false;}
};




/*
* GENERIC DECK CLASS
-> Holds a vector of cards (Card objects).
*/


class Deck
{
private:
	deque<Card*> drawPile;
	deque<Card*> discardPile;

	void initialize(string fileName);
	void populateDeck(ifstream& file);

	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar.template register_type<Card>();
		ar.template register_type<InfectionCard>();
		ar.template register_type<CityCard>();
		ar.template register_type<EventCard>();
		ar.template register_type<EpidemicCard>();
		ar.template register_type<RoleCard>();
		ar & drawPile & discardPile;
	}

public:
	Deck();
	Deck(string fileName);
	~Deck();

	deque<Card*> &getDrawPile();
	deque<Card*> &getDiscardPile();

	int cardsInDeck();
	void shuffleDeck();
	void discardToDraw(); //Adds shuffled discard pile to the top of the draw pile
	Card* drawFromTop();
	Card* drawFromBottom();	
	Card* getDiscardedCard(int index);
	void discard(Card* card);
	void addCard(Card* card);
	void insertEpidemicCards(int numberOfCards); // inserts Epidemic cards into different sectors of the deck
};




/*
* INFECTION CARD CLASS
*/

class InfectionCard : public Card {
private:
	string city; // The city infected
	InfectType color; // Based off an enum of InfectType, we get the color

	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<Card>(*this);
		ar & city & color;
	}
public:
	InfectionCard(); // Default constructor
	InfectionCard(string city, InfectType color); // Constructor setting all variables
	~InfectionCard(); // Default destructor
	void setCity(string city); // Setter for the city
	void setColor(InfectType color); // Setter for the color
	string getCity() const; // Getter for the city
	string getColorString() const; // Getter for the color as a string
	InfectType getColor() const; // Returns an InfectType for the color
	string print(); // Print method implementation from Card

	//Operator overload//
	const bool operator==(const string str) const;
};



// -------- EVENT CARDS
class EventCard : public Card {
private:
	string name; // Name for the Event Card
	string desc; // Description for the Event Card

	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<Card>(*this);
		ar & name & desc;
	}

public:
	EventCard(); // Default constructor
	EventCard(string name, string desc); // Constructor that sets all variables
	~EventCard(); // Default destructor
	void setName(string name); // Setter for the name
	void setDesc(string desc); // Setter for the description
	string getName() const; // Getter for the Event name
	string getDesc() const; // Getter for the Event description
	string print(); // Implementation for print from Card

	//Operator overload//
	const bool operator==(const string str) const;
};




// -------- CITY CARDS
class CityCard : public Card {
private:
	string city; // City name
	InfectType color; // Color 

	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<Card>(*this);
		ar & city & color;
	}

public:
	CityCard(); // Default constructor
	CityCard(string city, InfectType color); // Constructor that sets all variables
	~CityCard(); // Default destructor
	void setCity(string city); // Setter for the city
	void setColor(InfectType color); // Setter for the color
	string getCity() const; // Getter for the city
	string getColorString() const; // Getter for the color as a string
	InfectType getColor() const; // Getter for the color as an InfectType
	string print(); // Implementation for print from Card

	//Operator overload//
	const bool operator==(const string str) const;
};




// -------- EPIDEMIC CARDS
class EpidemicCard : public Card {
public:
	string desc; // Description (info) for the Epidemic Card

	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<Card>(*this);
		ar & desc;
	}

public:
	EpidemicCard(); // Default constructor
	EpidemicCard(string desc); // Constructor that sets the description
	~EpidemicCard(); // Default destructor
	void setDesc(string desc); // Setter for the description
	string getDesc() const; // Getter for the description
	string print(); // Implementation of print from Card
	string printDescription(); // Prints the description of the epidemic card
};




/*
* ROLE CLASS
* - Creates a Card of type Role
*/

class RoleCard : public Card {
private:
	string name; // Name of the role 
	string color; // Color for the role
	string desc; // Description for the role

	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<Card>(*this);
		ar & name & color & desc;
	}

public:
	RoleCard(); // Default constructor
	RoleCard(string name, string color, string desc); // Constructor that sets all member variables
	~RoleCard(); // Default destructor
	string getName() const; // Returns the role name
	string getColor() const; // Returns the role color
	string getDesc() const; // Returns the role description
	void setName(string name); // Sets the role name
	void setColor(string color); // Sets the role color
	void setDesc(string desc); // Sets the role description
	string print(); // Prints out the card (inherited)

	//Operator overload//
	const bool operator==(const string str) const;
};




/*
* REFERENCE CARD
* - Holds the Reference card for all players
*/

class ReferenceCard : public Card {
private:
	string reference; // The reference card
	
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & boost::serialization::base_object<Card>(*this);
		ar & reference;
	}

public:
	ReferenceCard(); // Default constructor
	~ReferenceCard(); // Default destructor
	
				  // NOTE: Do not need any other constructor since the reference card is always the same.
	string getReferenceCard() const; // Return the reference card
	string print(); // Print implementation (from Card)
};