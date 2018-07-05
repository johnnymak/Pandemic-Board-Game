#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <text_oarchive.hpp>
#include <text_iarchive.hpp>
#include <vector.hpp>
#include <deque.hpp>

using namespace std;

//GAME RULE CONSTANTS/////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
const int NUM_OF_DISEASES = 4;
const int NUM_OF_DISEASE_CUBES_PER_TYPE = 24;
const int MAX_CUBES_PER_CITY = 3;

const int MAX_OUTBREAKES = 8;
const int INFECTION_RATE[] = { 2,2,2,3,3,4,4 };
const int INITIAL_INFECTION = 3;
const int EPIDEMIC_INFECTION = 3;

const int MAX_HAND_SIZE = 7;
const int NUM_OF_ACTIONS = 4;

const int NUM_OF_CITIES = 48;
const int NUM_OF_RESEARCH_STATIONS = 6;

const int NUM_OF_EPIDEMIC_CARDS = 3;
const int NUM_OF_PLAYER_CARDS_DRAWN_PER_TURN = 2;
const int NUM_OF_CARDS_TO_CURE = 5;
const int NUM_OF_CARDS_TO_CURE_FOR_SCIENTIST = 4;
const int NUM_OF_CARDS_FOR_FORCAST = 6;

//GAME FILES/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

const string MAP_INITIAL_FILE = "../ReferenceFiles/InitialMap.txt";
const string INFECTION_CARD_INITIAL_FILE = "../ReferenceFiles/Infection.txt";
const string PLAYER_CARD_INITIAL_FILE = "../ReferenceFiles/Player.txt";
const string ROLE_CARD_INITIAL_FILE = "../ReferenceFiles/Role.txt";
const string SAVED_MAP_FILE = "../ReferenceFiles/SavedGame.txt";

//ROLES//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

const string DISPATCHER = "Dispatcher";
const string MEDIC = "Medic";
const string OPERATIONS = "Operations Expert";
const string PLANNER = "Contingency Planner";
const string QUARANTINE = "Quarantine Specialist";
const string RESEARCHER = "Researcher";
const string SCIENTIST = "Scientist";

//EVENT CARDS/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

const string AIRLIFT = "Airlift";
const string QUIET = "One Quiet Night";
const string FORECAST = "Forecast";
const string GOVERNMENT = "Government";
const string RESILIENT = "Resilient Population";

//ENUMS///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
enum InfectType : int{
	yellow = 0,
	red = 1,
	blue = 2,
	black = 3
};

string infectTypeEnumToString(InfectType t);

enum CureStatus {
	notCured,
	cured,
	eradicated
};

string cureStatusEnumToString(CureStatus c);