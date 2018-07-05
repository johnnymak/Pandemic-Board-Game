#include "InputOutput.h"

//Reads in input and handles boundary cases
//Returns a bool if input was valid or not
bool getIntInput(int & variable, int lowerbound, int upperbound)
{
	if (!(cin >> variable) || variable < lowerbound || variable > upperbound)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "ERROR: Invalid input." << endl;
		return false;
	}
	return true;
}

bool getEventInput(int &variable, int lowerbound, int upperbound) 
{
	if (!(cin >> variable) || variable < lowerbound || variable > upperbound) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return false;
	}
	return true;
}

bool getAnotherPlayersApproval(int otherPlayer)
{
	// Get other player's approval
	cout << "\nPlayer " << otherPlayer << " , do you approve this action? (y/n)" << endl;
	char answer;
	//Checks for valid input
	while (!(cin >> answer))
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "ERROR: Invalid input. Please try again." << endl;
	}


	//If approved Transfer card
	if (toupper(answer) == 'Y') {
		return true;
	}
	else {
		return false;
	}
}

// Handles event card prompting at key interval times
// Return: T/F to see if we want to perform an action
bool promptEvent(int &playerID, int numOfPlayers)
{
	int input;

	cout << "\nWould any player like to play an Event Card? Enter the player # (1, 2, 3, or 4).\nIf not, enter any other key" << endl;
	cout << "> ";

	// NOW HANDLE WHICH PLAYER CHOSE TO PLAY AN EVENT CARD & HANDLE EVENT CARD ACTIONS
	if (getEventInput(input, 1, numOfPlayers)) {
		cout << "\nPlayer #" << input << " decided to use an Event Card." << endl;
		playerID = input;
		return true;
	}

	// Default: no action to take.
	return false;
}

// Pause Game Function
void pause()
{
	char key;
	cout << "\nReady to continue? (Enter any key to continue.)" << endl;
	cout << "> ";
	cin >> key;
}