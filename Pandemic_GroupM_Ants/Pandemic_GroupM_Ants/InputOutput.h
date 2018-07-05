#pragma once

#include "CommonLibrary.h"

////////////////////////////////////////////////////////////////////////////
//Collection of free helper input and output functions
////////////////////////////////////////////////////////////////////////////

//collects integer input from user with error checking and boundary checking
//returns false if value is unacceptable
bool getIntInput(int & variable, int lowerbound, int upperbound);

//collects integer input from user with error checking and boundary checking
//returns false if the prompt for an event card was any other key
bool getEventInput(int &variable, int lowerbound, int upperbound);

//asks a player if they approve of changes another player is making
//returns otherPlayer's answer
bool getAnotherPlayersApproval(int otherPlayer);

//Displays "Press any key to continue" and waits for input
void pause();

//Event Card prompt
bool promptEvent(int &playerID, int numOfPlayers);