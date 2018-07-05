#pragma once

#include "CommonLibrary.h"
#include "Subject.h"

//City Node Class //////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CityNode : public Subject
{
	//Attributes//////////////////////////////
	string name;
	InfectType area;
	int infectionLevel[NUM_OF_DISEASES];
	vector<CityNode*> connections;
	bool researchStation;

	//Serialize//////////////////
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar & name & area & infectionLevel & researchStation & connections;
	}

public:
	//Constructors////////////////////////////
	CityNode();
	CityNode(string name, InfectType area, bool researchStation = false);
	~CityNode();

	//Get and Set////////////////////////////
	string getName() {	return this->name;	}
	
	InfectType getArea() { return this->area; }

	int getInfectionLevel(InfectType infection) { return this->infectionLevel[infection]; }

	// OBSERVER PATTERN: Increase Infection Level
	void increaseInfectionLevel(InfectType infection);
	// OBSERVER PATTERN: Reduce Infection Level 
	void reduceInfectionLevel(InfectType infection);
	// OBSERVER PATTERN: Add Research Station
	void addResearchStation();

	bool hasResearchStation() { return this->researchStation; }
	
	
	vector<CityNode*> getConnections() { return this->connections; }
	void addConnection(CityNode* city) { this->connections.push_back(city); }
};

//GAME MAP CLASS///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GameMap : public Subject
{
	//Attributes/////////////////////////////////////
	CityNode* head;
	vector<CityNode*> cityList;

	//Serialize/////////////////////////
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned version) {
		ar.template register_type<CityNode>();
		ar & head & cityList;
	}

public:
	//Constructors///////////////////////////////////
	GameMap();
	GameMap(string filename);
	~GameMap();

	//Get and Set////////////////////////////////////
	CityNode* getHead() { return this->head; }
	CityNode* getCity(string name);
	CityNode* getCity(int index) {return cityList.at(index);}
	int getNumberOfCities() { return cityList.size();}
	vector<CityNode*> * getCityList();

	//outputs all cities for selection by user
	void showAllCityOptions();

private:
	//Helper Functions///////////////////////////////
	void loadStartingMap(string filename);

};
