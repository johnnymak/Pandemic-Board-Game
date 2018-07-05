#pragma once

#include "CommonLibrary.h"
#include "Player.h"
#include "Observer.h"


using namespace std;

#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

class PlayerView : public Observer {

	public: 
		// Constructor
		PlayerView(vector<Player*>* player);

		// Destructor
		~PlayerView();

		// Overriden Observer Function
		void Update(string message);

		// ========== Playerview Functions =============
		void printPlayerDisplay(int index) const; // Prints the player view
		void clearPlayerDisplay();  // Clears the player view

		void setActivePlayer(int index) {
			playerIndex = index;
		}

	private:	

		// Subject to be attached
		vector<Player*> subjPlayers;

		int playerIndex;
};

#endif