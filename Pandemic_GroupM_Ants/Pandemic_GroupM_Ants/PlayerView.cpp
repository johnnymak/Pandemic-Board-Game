
#include "PlayerView.h"
#include "MapView.h"
#include "CommonLibrary.h"

// Constructor
PlayerView::PlayerView(vector<Player*>* player) {

	for (int i = 0; i < player->size(); i++) {
		this->subjPlayers.push_back(player->at(i));
		this->subjPlayers.at(i)->Attach(this);
	}
}

// Destructor
PlayerView::~PlayerView() {	}


// Overriden Observer Pattern: Update(message)
void PlayerView::Update(string message) {

	try {
		string number = message.substr(7, 1);
		int changedPlayer = atoi(number.c_str());
		if (changedPlayer == playerIndex) {
			regex re2("(.*) moved");
			if (!regex_match(message, re2)) {
				cout << endl;
				printPlayerDisplay(this->playerIndex);
			}
		}
		//else do nothing.
	}
	//Message not from Player model
	catch (const std::out_of_range& oor) {
		printPlayerDisplay(this->playerIndex);
	}	
}

// Print PLayerView Function
void PlayerView::printPlayerDisplay(int index) const{

	cout << " ************************************************************************************************************************** " << endl;
	cout << "   Player #" << subjPlayers.at(index)->getId() + 1 << "- Location: " << subjPlayers.at(index)->getPawn()->getName() << endl;
	cout << "\n   " << subjPlayers.at(index)->getRole()->print() << endl;
	cout << "\n   " << subjPlayers.at(index)->printHand() << endl;
	cout << " **************************************************************************************************************************\n " << endl;
}

// Clear Player Display
void PlayerView::clearPlayerDisplay() {
	system("CLS");
}
