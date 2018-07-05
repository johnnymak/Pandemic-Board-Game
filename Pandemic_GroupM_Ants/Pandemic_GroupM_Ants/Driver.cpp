#include "CommonLibrary.h"
#include "GameController.h"

#include "MapView.h"
#include "PlayerView.h"
#include "Cards.h"

int main()
{
	GameController game;
	//Welcome Screen
	cout << endl << endl << endl;
	cout << "          ==================================================================================================================================================================================================================" << endl;
	cout << "          ||                                                                                                                                                                                                              ||" << endl;
	cout << "          ||                                                               __________                     .___                 .__                                                                                        ||" << endl;
	cout << "          ||                                                               \\______   \\_____     ____    __| _/  ____    _____  |__|  ____                                                                                 ||" << endl;
	cout << "          ||                                                                |     ___/\\__  \\   /    \\  / __ | _/ __ \\  /     \\ |  |_/ ___\\                                                                                ||" << endl;
	cout << "          ||                                                                |    |     / __ \\_|   |  \\/ /_/ | \\  ___/ |  Y Y  \\|  |\\  \\___                                                                                ||" << endl;
	cout << "          ||                                                                |____|    (____  /|___|  /\\____ |  \\___  >|__|_|  /|__| \\___  >                                                                               ||" << endl;
	cout << "          ||                                                                               \\/      \\/      \\/      \\/       \\/          \\/                                                                                ||" << endl;
	cout << "          ||                                                                                                                                                                                                              ||" << endl;
	cout << "          ==================================================================================================================================================================================================================" << endl << endl;
	



	//Request: new or saved game?
	int answer;
	cout << "Would you like to:" << endl;
	cout << "1) Start a new game" << endl;
	cout << "2) Load a saved game" << endl;

	// Input Tick for visual purposes 
	cout << "> ";
	
	//Handle unpredictable answers
	while (!(cin >> answer) || answer < 1 || answer > 2)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input, try again" << endl;
	}
	//Start new game
	if (answer == 1) {
		game.initializeNewGame();
	}
	//Load saved game
	else if (answer == 2) {
		game.loadGame();
	}

	//GameLoop
	while (true)			//Iteration #2//issue, need to stop immediately if lose/win game//
	{
		game.startRound();
		game.saveGame();

		//Request: continue or quit?
		cout << "The last game round has been saved, would you like to:" << endl;
		cout << "1) Continue" << endl;
		cout << "2) Quit" << endl;
		//Check input
		while (!(cin >> answer) || answer < 1 || answer > 2)
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input, try again" << endl;
			answer = 2;
		}
		
		if (answer == 2)
			break;
	}
}