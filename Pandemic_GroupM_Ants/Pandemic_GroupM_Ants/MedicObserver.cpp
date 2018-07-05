#include "MedicObserver.h"

MedicObserver::MedicObserver(Player * player, GameMap * map, Markers* markers) : player(player), map(map), markers(markers)
{
	player->Attach(this);
}

void MedicObserver::Update(string message)
{
	//check if any diseases are cured
	for (int i = 0; i < NUM_OF_DISEASES; i++) {
		InfectType infection = static_cast<InfectType>(i);
		if (markers->getCureMarker(infection) == cured) {
			CityNode* city = player->getPawn();
			int cubesToRemove = city->getInfectionLevel(infection);
			//remove all cubes of the disease in the player's city
			for (int i = 0; i < cubesToRemove; i++) {
				city->reduceInfectionLevel(infection);
				markers->addDiseaseCube(infection);
			}
		}
	}
}

