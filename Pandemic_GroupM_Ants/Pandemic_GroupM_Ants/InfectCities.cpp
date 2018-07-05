#include "InfectCities.h"

vector<string> InfectCities::outbreakHistory;

//Adds a disease cube to a city
void InfectCities::infectCity(CityNode* city, InfectType infection, Markers* markers, vector<Player*> playerList, bool firstCall)
{
	if (firstCall) {
		outbreakHistory.clear();
	}
	if (!InfectCities::quarantineCheck(city, playerList, markers->getCureMarker(infection) == cured)) {
		//Check if city can be infected
		if (city->getInfectionLevel(infection) < MAX_CUBES_PER_CITY) {
			//Infect city, add cube of infection color to city
			if (markers->useDiseaseCube(infection)) {
				city->increaseInfectionLevel(infection);
				cout << "A " << infectTypeEnumToString(infection) << " cube has been added to " << city->getName() << endl;
			}
			//All disease cubes used
			else {
				//game over //handled in game controller
				return;
			}
		}
		//Outbreak occurs, city already had three disease cubes of infection color
		//All connecting cities will be infected.
		else {
			//track cities that have had an outbreak this chain.
			outbreakHistory.push_back(city->getName());
			InfectCities::handleOutbreak(city, infection,markers, playerList);
		}
	}
	//Quarentine check was true --> don't infect
	else {
		cout << "City is protected" << endl;
	}

}


//Checks if city is next to a Quarentine Specialist or a Medic
//Returns true, if city affected by Quarentine Specialist
bool InfectCities::quarantineCheck(CityNode * city, vector<Player*> playerList, bool isCured)
{

	for (auto const& player : playerList) {
		//Check if a player is a Quarentine expert OR Medic with an cured infection
		if (*player == QUARANTINE || (*player == MEDIC && isCured)) {
			//Checks to see if city to infect matches any connected city
			for (auto const& connectedCity : player->getPawn()->getConnections()) {
				if (city == connectedCity) {
					return true;
				}
			}
			//Checks if city to infect matches current city
			if (player->getPawn() == city) {
				return true;
			}
		}
	}
	//No conflicts
	return false;
}

//Handles Outbreak events
void InfectCities::handleOutbreak(CityNode * city, InfectType infection, Markers* markers, vector<Player*> playerList)
{
	//increase counter and check if it has reached maximum
	markers->increaseOutbreakCounter();
	if (markers->getOutbreakCounter() == MAX_OUTBREAKES) {
		//game over //handled in the game Controller
		return;
	}

	cout << "\n=========== An outbreak has occurred in " << city->getName() << " ===========" << endl;

	//Infect all connected cities.
	vector<CityNode*> connections = city->getConnections();
	for (int i = 0; i < connections.size(); i++) {
		bool match = false;
		//Check if an outbreak has already occured here this chain
		for (int j = 0; j < outbreakHistory.size(); j++) {
			if (connections[i]->getName() == outbreakHistory[j])
			{
				match = true;
			}
		}
		if (!match) {
			InfectCities::infectCity(connections[i], infection, markers, playerList, false);
		}
	}
}