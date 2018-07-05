#include "Strategy.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//----- Strategy Class Common Functions------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////

//Selects which players pawn will be moved. Handles special Dispatcher case
void Strategy::pawnSelection(vector<Player*> playerList, int activePlayer, int &movingPawn, bool isAirlift)
{
	//Check if the player a dispatcher
	if (*playerList[activePlayer] == DISPATCHER || isAirlift) {
		bool selectionApproved = false;
		while (!selectionApproved) {
			//Ask which pawn they wish to move
			cout << "Which player's pawn would you like to move?" << endl;
			for (int i = 0; i < playerList.size(); i++) {
				if (i != activePlayer) {
					cout << i + 1 << "- Player " << i + 1 << endl;
				}
				else {
					cout << i + 1 << "- Your Pawn " << endl;
				}
			}
			
			// Input Tick for visual purposes 
			cout << "> ";

			int playerPawn;
			//Checks for valid input
			while (!getIntInput(playerPawn, 1, playerList.size() + 1));

			//If player did not select their own pawn
			if (--playerPawn != activePlayer) {

				//If other Player approves exit the loop
				if (getAnotherPlayersApproval(playerPawn + 1)) {
					movingPawn = playerPawn;
					selectionApproved = true;
				}
				else {
					cout << "The other player has rejected your request." << endl;
				}
			}
			//Player selected themselves
			else {
				movingPawn = activePlayer;
				selectionApproved = true;
			}
		}
	}
	//Player is not the dispatcher or the event played is not the Airlift card
	else {
		movingPawn = activePlayer;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ DRIVE FERRY CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Player can move to a connecting city
bool DriveFerry::execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	//Finds the pawn which is to be moved, handles Dispatcher special case
	int playerOfPawn;
	pawnSelection(playerList, activePlayer, playerOfPawn, false);

	cout << "Drive / Ferry Selected: Where would you like to go? " << endl;
	int newLocation;
	vector<CityNode*> connections = playerList[playerOfPawn]->getPawn()->getConnections();

	// Check for possible nearby cities 
	for (int i = 0; i < connections.size(); i++) {
		cout << "(" << i + 1 << ") " << connections[i]->getName() << "     ";
	}

	// Input Tick for visual purposes 
	cout << "\n> ";

	//Check input
	if (!getIntInput(newLocation, 1, connections.size() + 1)) {
		return false;
	}

	// Change location of the player
	MovePawn command(playerList[playerOfPawn], connections[newLocation - 1]);
	command.execute();
	cout << "Player " << playerOfPawn + 1 << " is now in " << playerList[playerOfPawn]->getPawn()->getName() << endl << endl;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ DIRECT FLIGHT CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Player can discard a city card to fly directly to that city
bool DirectFlight::execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	//Finds the pawn which is to be moved, handles Dispatcher special case
	int playerOfPawn;
	pawnSelection(playerList, activePlayer, playerOfPawn, false);

	cout << "Direct Flight Selected: Where would you like to go? (To go back to menu enter non-displayed character)" << endl;
	int newLocation;
	vector<CityNode*> connections;
	vector<int> indexes;
	//go through hand and find city cards and add them to possible connections
	for (int l = 0; l < playerList[activePlayer]->getNumOfCards(); l++)
	{
		if (playerList[activePlayer]->getCard(l)->getType() == "City")
		{
			CityCard *card = dynamic_cast<CityCard*>(playerList[activePlayer]->getCard(l));
			connections.push_back(map->getCity(card->getCity()));
			indexes.push_back(l);
		}
	}

	if (connections.size() == 0)
	{
		cout << "You have no city cards in your hand, You cannot use Direct Flight";
		return false;
	}
	//print out where the player can fly
	for (int k = 0; k < connections.size(); k++) {
		cout << " (" << k + 1 << ")" << connections[k]->getName() << "     ";
	}

	// Input Tick for visual purposes 
	cout << "\n> ";

	//check input
	if (!getIntInput(newLocation, 1, connections.size() + 1)) {
		return false;
	}

	//Take actions
	if (playerOfPawn == activePlayer) {
		DiscardCardAndMove command(playerList[activePlayer], connections[newLocation - 1], indexes[newLocation-1], playerDeck);
		command.execute();
	}
	else {
		MovePawn command1(playerList[playerOfPawn], connections[newLocation - 1]);
		DiscardCard command2(playerList[activePlayer], &indexes[newLocation - 1], 1, playerDeck);
		command1.execute();
		command2.execute();
	}
	cout << "Player " << playerOfPawn + 1 << " is now in " << playerList[playerOfPawn]->getPawn()->getName() << endl << endl;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ CHARTER FLIGHT CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Player can discard a card matching their current city and move anywhere
bool CharterFlight::execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	//Finds the pawn which is to be moved, handles Dispatcher special case
	int playerOfPawn;
	pawnSelection(playerList, activePlayer, playerOfPawn, false);

	cout << "Charter Flight Selected: Where would you like to go?" << endl;
	int newLocation;
	int cardIndex;
	// Checks hand for City Card
	if (cardIndex = playerList[activePlayer]->checkifPlayerHasCard(playerList[playerOfPawn]->getPawn()->getName()) != -1) {
		//List all cities
		map->showAllCityOptions();

		// Input Tick for visual purposes 
		cout << "> ";

		//Check input
		if (!getIntInput(newLocation, 0, NUM_OF_CITIES)) {
			return false;
		}

		//Take actions
		if (playerOfPawn == activePlayer) {
			DiscardCardAndMove command(playerList[activePlayer], map->getCityList()->at(newLocation), cardIndex, playerDeck);
			command.execute();
		}
		else {
			MovePawn command1(playerList[playerOfPawn], map->getCityList()->at(newLocation));
			DiscardCard command2(playerList[activePlayer], &cardIndex, 1, playerDeck);
			command1.execute();
			command2.execute();
		}

		cout << "Player " << playerOfPawn + 1 << " is now in " << playerList[playerOfPawn]->getPawn()->getName() << endl << endl;
		return true;
	}
	// Player does not have card in hand
	else
	{
		cout << "Sorry, Charter Flight is not possible! You don't have the card matching the city you are in." << endl;
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ SHUTTLE FLIGHT CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//A Player in a city with a Research Station can move to any other city with a Research Station
bool ShuttleFlight::execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	//Finds the pawn which is to be moved, handles Dispatcher special case
	int playerOfPawn;
	pawnSelection(playerList, activePlayer, playerOfPawn, false);

	// Check whether player is at a research station
	if (!playerList[playerOfPawn]->getPawn()->hasResearchStation()) {
		cout << "Pawn is not currently at a research station! " << endl;
		return false;
	}

	//Special Case: If player is an operations expert they have the option to go anywhere
	if (*playerList[activePlayer] == OPERATIONS) {
		//List all cities
		cout << "Shuttle Flight Selected: Where would you like to go?" << endl;
		map->showAllCityOptions();

		// Input Tick for visual purposes 
		cout << "> ";

		//Check input
		int newLocation;
		if (!getIntInput(newLocation, 0, NUM_OF_CITIES)) {
			return false;
		}

		// Change the location of the player
		MovePawn command(playerList[playerOfPawn], map->getCityList()->at(newLocation));
		command.execute();
		cout << "Player " << playerOfPawn + 1 << " is now in " << playerList[playerOfPawn]->getPawn()->getName() << endl << endl;
		return true;
	}


	//Default Case///////////////////////////////////
	cout << "Shuttle Flight Selected: Where would you like to go? " << endl;
	vector<CityNode*>* cityList = map->getCityList();
	vector<CityNode*> researchStation;

	// Goes through every city and check if it has a research station
	for (int i = 0; i < cityList->size(); i++) {
		if (cityList->at(i)->hasResearchStation() && cityList->at(i) != playerList[activePlayer]->getPawn()) {
			researchStation.push_back(cityList->at(i));
		}
	}

	// Print out list of cities with research station
	for (int i = 0; i < researchStation.size(); i++) {
		cout << researchStation[i]->getName() << " (" << i << ")" << endl;
	}

	// Input Tick for visual purposes 
	cout << "> ";

	//Check input
	int newLocation;
	if (!getIntInput(newLocation, 0, researchStation.size())) {
		return false;
	}

	// Move Player to the City 
	MovePawn command(playerList[playerOfPawn], researchStation[newLocation]);
	command.execute();
	cout << "Player " << playerOfPawn + 1 << " is now in " << playerList[playerOfPawn]->getPawn()->getName() << endl << endl;
	return true;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ DISPATCHER MOVE CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Dispatcher ONLY, can move any players pawn to the location of another players pawn.
bool DispatcherMovement::execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	if (!(*playerList[activePlayer] == DISPATCHER)) {
		cout << "You are not the Dispatcher and cannot make this move." << endl;
		return false;
	}
	//Active Player is the Dispatcher
	else {
		//Finds the pawn which is to be moved
		int playerOfPawn;
		pawnSelection(playerList, activePlayer, playerOfPawn, false);

		//Asks which player to move the pawn to
		cout << "To which player would you like to move player " << playerOfPawn + 1 << "'s pawn?" << endl;
		for (int i = 0; i < playerList.size(); i++) {
			if (i != playerOfPawn) {
				cout << i+1 << " - Player " << i + 1 << endl;
			}
		}

		// Input Tick for visual purposes 
		cout << "> ";

		//Checks for valid input
		int otherPlayer;
		if (!getIntInput(otherPlayer, 1, playerList.size() + 1)) {
			return false;
		}
		otherPlayer--;

		//Move Player
		MovePawn command(playerList[playerOfPawn], playerList[otherPlayer]->getPawn());
		command.execute();
		cout << "Player " << playerOfPawn + 1 << " is now in " << playerList[playerOfPawn]->getPawn()->getName() << endl << endl;
		return true;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ BUILD RESEARCH STATION CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Player can build a research station in their current city if they discard the matching city card.
bool BuildResearchStation::execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	cout << "Build a Research Station Selected" << endl;

	//Check if there is no research station here
	if (playerList[activePlayer]->getPawn()->hasResearchStation()) {
		cout << "There is already a research station here." << endl;
		return false;
	}

	//Special Case: Operations Expert Players can build without discarding a matching city card
	if (*playerList[activePlayer] == OPERATIONS) {
		if (markers->useResearchStation()) {
			playerList[activePlayer]->getPawn()->addResearchStation();
			cout << "Research Station Added! ";
			return true;
		}
		else {
			cout << "No more Research Stations to be built." << endl;
			return false;
		}
	}
	//Default Case: Player discards a matching city card to build a research station
	else {
		//Check if player has the card for the city they are in
		int index;
		if ((index = playerList[activePlayer]->checkifPlayerHasCard(playerList[activePlayer]->getPawn()->getName())) != -1) {
			//Build a research station
			if (markers->useResearchStation()) {
				playerList[activePlayer]->getPawn()->addResearchStation();
				cout << "Research Station Added! " << endl;
				DiscardCard command(playerList[activePlayer], &index, 1, playerDeck);
				command.execute();
				cout << "Research Station Added! " << endl;
				return true;
			}
			//No research station markers left
			else {
				cout << "No more Research Stations to be built." << endl;
				return false;
			}
		}
		//Player does not have the card for the city they are in
		else {
			cout << "You cannot build a Research Station here, you do not have a matching card." << endl;
			return false;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ DISCOVER CURE CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//By Discarding 5 cards of the same color, the player can cure the disease of that color
bool DiscoverCure::execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	//Check if player is at a city with a research station.
	if (playerList[activePlayer]->getPawn()->hasResearchStation()) {
		int numberToCure;
		//Special Case: Scientist only need 4 cards to cure
		if (*playerList[activePlayer] == SCIENTIST) {
			numberToCure = NUM_OF_CARDS_TO_CURE_FOR_SCIENTIST;
		}
		//Default Case: Players need 5 cards to cure
		else {
			numberToCure = NUM_OF_CARDS_TO_CURE;
		}

		//Display Player Card Options
		cout << "Please select " << numberToCure << " cards of the same color." << endl;
		playerList[activePlayer]->displayPlayerCardOptions();

		//Player selects cards for cure
		int count[NUM_OF_DISEASES] = { 0 };
		int* answer = new int[numberToCure];
		fill_n(answer, numberToCure, -1);
		for (int i = 0; i < numberToCure; i++) {

			// Input Tick for visual purposes 
			cout << "> ";

			//Checks for valid input
			if (!getIntInput(answer[i], 0, playerList[activePlayer]->getNumOfCards())) {
				return false;
			}

			//Check if Valid Choice
			for (int j = 0; j <= i; j++) {
				//Check that card has not already been selected
				if (i != j && answer[i] == answer[j]) {
					cout << "That card has already been selected, you need to start over." << endl;
					return false;
				}
				//Check that a city card has been selected
				if (playerList[activePlayer]->getCard(answer[j])->getType() != "City") {
					cout << "That is not a city card" << endl;
					return false;
				}
			}
			count[dynamic_cast<CityCard*>(playerList[activePlayer]->getCard(answer[i]))->getColor()]++;
		}

		//Check if requirements have been met and cure 
		for (int i = 0; i < NUM_OF_DISEASES; i++) {
			if (count[i] == numberToCure) {

				DiscardCard command(playerList[activePlayer], answer, numberToCure, playerDeck);
				command.execute();

				//Cure Disease
				markers->cureDiseaseUpdateMarker(static_cast<InfectType>(i));
				cout << "The " << infectTypeEnumToString(static_cast<InfectType>(i)) << " disease has been cured." << endl;
				return true;
			}
		}
		cout << "Cannot cure any disease with these cards." << endl;
		return false;
	}
	//Player is not at a city with a research station
	else {
		cout << "You must be at a research station to cure a disease!" << endl;
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ TREAT DISEASE CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Player removes cubes of a disease from a city
bool TreatDisease::execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	//Player chooses which disease to treat
	for (int i = 0; i < NUM_OF_DISEASES; i++) {
		cout << i << ") " << infectTypeEnumToString(static_cast<InfectType>(i)) << endl;
	}
	int answer;

	// Input Tick for visual purposes 
	cout << "> ";

	//Checks for valid input
	if (!getIntInput(answer, 0, NUM_OF_DISEASES)) {
		return false;
	}

	InfectType infection = static_cast<InfectType>(answer);
	CityNode* city = playerList[activePlayer]->getPawn();

	//If Disease is cured or Player is Medic - remove all cubes of color
	if (markers->getCureMarker(infection) == cured || *playerList[activePlayer] == MEDIC) {
		int cubesToRemove = city->getInfectionLevel(infection);
		for (int i = 0; i < cubesToRemove; i++) {
			city->reduceInfectionLevel(infection);
			markers->addDiseaseCube(infection);
		}
		checkifDiseaseNowEradicated(markers, infection);
		cout << "All blocks of " << infectTypeEnumToString(infection) << " disease have been removed." << endl;
		return true;
	}
	//If Disease is not cured - remove one cube of color
	else if (city->getInfectionLevel(infection) > 0) {
		city->reduceInfectionLevel(infection);
		markers->addDiseaseCube(infection);
		checkifDiseaseNowEradicated(markers, infection);
		cout << "One block of " << infectTypeEnumToString(infection) << " disease has been removed." << endl;
		return true;
	}
	//No cubes in city to remove
	else {
		cout << "No infection of color " << infectTypeEnumToString(infection) << " here to cure." << endl;
		return false;
	}
}

void TreatDisease::checkifDiseaseNowEradicated(Markers* markers, InfectType infection)
{
	if (markers->getCureMarker(infection) == cured) {
		//If there are no more cubes on the map the disease is eradicated
		if (markers->getNumOfDiseaseCubes(infection) == NUM_OF_DISEASE_CUBES_PER_TYPE) {
			markers->eradicateDiseaseUpdateMarker(infection);
			cout << infectTypeEnumToString(infection) << " Disease has now been eradicated!" << endl;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ SHARE KNOWLEDGE CLASS --------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Player can give other player a card if in the same city, and the card matches the city
bool ShareKnowledge::execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	int cardToPass = -1;
	int otherPlayer = -1;
	cout << "Share Knowledge Selected." << endl;

	//Ask who they would like to share knowledge with
	cout << "Which player would you like to share knowlege with?" << endl;
	for (int i = 0; i < playerList.size(); i++) {
		if (i != activePlayer) {
			cout << "- Player " << "(" << i + 1 << ")" << endl;
		}
	}

	// Input Tick for visual purposes 
	cout << "> ";

	//Checks for valid input
	if (!getIntInput(otherPlayer, 1, playerList.size() + 1)) {
		return false;
	}
	otherPlayer--;

	// Checks if players are in the same city
	if (playerList[activePlayer]->getPawn() != playerList[otherPlayer]->getPawn()) {
		cout << "Sorry, you and the other player are not in the same city, you cannot share a card." << endl;
		return false;
	}

	//Special Case: Researcher can give any card
	if (*playerList[activePlayer] == RESEARCHER) {
		cout << "\nPlease select the card to pass." << endl;
		playerList[activePlayer]->displayPlayerCardOptions();

		// Input Tick for visual purposes 
		cout << "> ";

		//Checks for valid input
		if (!getIntInput(cardToPass, 0, playerList[activePlayer]->getNumOfCards())) {
			return false;
		}
		//If approved Transfer card
		if (getAnotherPlayersApproval(otherPlayer + 1)) {
			transferCard(cardToPass, playerList[activePlayer], playerList[otherPlayer], playerDeck);
		}
		return true;
	}
	//Special Case: Player can take any card from the Researcher
	else if (*playerList[otherPlayer] == RESEARCHER) {
		//Display Player Card Options
		cout << "Please select the card to pass." << endl;
		playerList[otherPlayer]->displayPlayerCardOptions();

		// Input Tick for visual purposes 
		cout << "> ";

		//Checks for valid input
		if (!getIntInput(cardToPass, 0, playerList[otherPlayer]->getNumOfCards())) {
			return false;
		}
		//If approved Transfer card
		if (getAnotherPlayersApproval(otherPlayer + 1)) {
			transferCard(cardToPass, playerList[otherPlayer], playerList[activePlayer], playerDeck);
			return true;
		}
	}
	//Default Case: Can pass the card that matches the city location
	else {

		// Searches active players hand for City cards that match location
		int i;
		if ((i = playerList[activePlayer]->checkifPlayerHasCard(playerList[activePlayer]->getPawn()->getName())) != -1) {
			cout << "You have the card for this city, you may pass it." << endl;
			//If approved Transfer card
			if (getAnotherPlayersApproval(otherPlayer + 1)) {
				transferCard(i, playerList[activePlayer], playerList[otherPlayer], playerDeck);
				return true;
			}
		}
		//Searches other players hand for City cards that match location
		else if ((i = playerList[otherPlayer]->checkifPlayerHasCard(playerList[activePlayer]->getPawn()->getName())) != -1) {
			cout << "The other player has the card for this city, they may pass it." << endl;
			//If approved Transfer card
			if (getAnotherPlayersApproval(otherPlayer + 1)) {
				transferCard(i, playerList[otherPlayer], playerList[activePlayer], playerDeck);
				return true;
			}
		}
		//No cards matching location found
		else {
			cout << "Neither of you have the card for " << playerList[activePlayer]->getPawn()->getName() << " and so cannot share." << endl;
			return false;
		}
	}
	//The transfer was rejected
	cout << "The other player has rejected your request" << endl;
	return false;
}

//Transfers the card from one players hand to another and checks player hand size
void ShareKnowledge::transferCard(int cardToPass, Player* fromPlayer, Player* toPlayer, Deck* playerDeck) {
	TransferCard command(fromPlayer, toPlayer, cardToPass, playerDeck);
	command.execute();
	cout << "You have successfully passed the card." << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ SHOW REFERENCE CARD CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Player views Reference Card description
bool ShowRefCard::execute(GameMap * map, Markers * markers, vector<Player*> playerList, Deck * playerDeck, Deck * infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	system("cls");

	ReferenceCard refCard;
	cout << refCard.print() << endl;
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ TAKE EVENT CARD CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TakeEventCard::execute(GameMap * map, Markers * markers, vector<Player*> playerList, Deck * playerDeck, Deck * infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	if (!(*playerList[activePlayer] == PLANNER)) {
		cout << "You are not the Contingency Planner and cannot make this move." << endl;
		return false;
	}
	//Active Player is the Planner
	else {
		deque<Card*> discardPile = playerDeck->getDiscardPile();
		//check if there are any cards to pick from
		if (discardPile.size() == 0) {
			cout << "There are no event cards in the discard pile." << endl;
			return false;
		}
		else {
			//Display choices
			cout << "Which card would you like to take?" << endl;
			for (int i = 0; i < discardPile.size(); i++) {
				cout << i << ") " << discardPile[i]->print() << endl;
			}

			// Input Tick for visual purposes 
			cout << "> ";

			//Get valid input
			int answer;
			if (!getIntInput(answer, 0, discardPile.size())) {
				return false;
			}
			else {
				//take card
				addCardToPocket command(playerList[activePlayer], playerDeck->getDiscardedCard(answer));
				command.execute();
				cout << "Contingency Planner took an event card, this card does not count against your hand limit." << endl;
				return true;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------DO NOTHING CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Keeps program from crashing when player chooses not to act
bool DoNothing::execute(GameMap * map, Markers * markers, vector<Player*> playerList, Deck * playerDeck, Deck * infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------DRAW CARD CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DrawCard::execute(GameMap * map, Markers * markers, vector<Player*> playerList, Deck * playerDeck, Deck * infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	//Draw a card
	Card* card = playerDeck->drawFromTop();
	cout << "\nPlayer " << activePlayer + 1 << " has drawn a " << card->getType() << " card" << endl;

	//Check if epidemic card
	if (card->getType() == "Epidemic") {
		epidemicCardActions(map, markers, playerList, infectionDeck);
		delete card;
	}
	//If a city or event card -> Add to hand
	else {
		cout << "The card has been added to player " << activePlayer + 1 << "'s hand" << endl;
		cout << "\t- " << card->print() << endl;
		AddCardToHand command(playerList[activePlayer], card, playerDeck);
		command.execute();
	}
	return true;
}

//called when player draws an epidemic card
void DrawCard::epidemicCardActions(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* infectionDeck)
{
	cout << "\n=========== An Epidemic Card has been Pulled ===========" << endl << endl;
	//Increase infection Rate
	markers->increaseInfectRate();
	cout << "Infection Rate has increased to " << markers->getInfectionRate() << endl;

	//Add 3 cubes to a city or until an outbreak occurs
	InfectionCard* card = dynamic_cast<InfectionCard*>(infectionDeck->drawFromBottom());
	cout << "Infection Card drawn - " << card->print() << endl;
	CityNode* city = map->getCity(card->getCity());
	InfectType infection = card->getColor();
	infectionDeck->discard(card);
	int outbreakCounterBefore = markers->getOutbreakCounter();

	for (int i = 0; i < EPIDEMIC_INFECTION || markers->getOutbreakCounter() != outbreakCounterBefore; i++) {
		InfectCities::infectCity(city, infection, markers, playerList, true);
	}

	//Ask event card////////////////////////////////////////////////////

	//Reshuffle discard pile and add to top of draw pile
	infectionDeck->discardToDraw();
	cout << "The infection deck discard pile has been shuffled and added to the top of the draw pile." << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ INFECT CLASS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Infect::execute(GameMap * map, Markers * markers, vector<Player*> playerList, Deck * playerDeck, Deck * infectionDeck, int activePlayer, bool &quietNightPlayed)
{
	InfectionCard* card = dynamic_cast<InfectionCard*>(infectionDeck->drawFromTop());
	if (markers->getCureMarker(card->getColor()) != eradicated) {
		cout << "- " << card->print() << endl;
		InfectCities::infectCity(map->getCity(card->getCity()), card->getColor(), markers, playerList, true);
		cout << endl;
	}
	infectionDeck->discard(static_cast<Card*>(card));
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------ EVENT CARDS ------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DoEventCards::execute(GameMap* map, Markers* markers, vector<Player*> playerList, Deck* playerDeck, Deck* infectionDeck, int playerID, bool &quietNightPlayed) {
	// Print the player's hand
	cout << "Enter the number associated to the Event Card found in your hand." << endl;
	playerList[playerID - 1]->displayPlayerCardOptions();

	int choice = 0; // Input choice

	// Validate input
	if (getIntInput(choice, 0, playerList[playerID - 1]->getNumOfCards())) {
		cout << "Your choice was option #" << choice << endl;

		// CASE: AIRLIFT
		if (playerList[playerID - 1]->checkifPlayerHasCardAtIndex(AIRLIFT, choice)) {
			cout << "Performing the Airlift event..." << endl;
			//Finds the pawn which is to be moved
			int playerOfPawn;
			cout << "Player ID: " << playerID << endl;
			pawnSelection(playerList, playerID - 1, playerOfPawn, true);
			cout << "Player ID: " << playerID << endl;

			//Asks which player to move the pawn to
			cout << "To which city would you like to move player " << playerOfPawn + 1 << "'s pawn?\nEnter the associated number." << endl;
			map->showAllCityOptions();

			int citychoice = 0;
			if (getIntInput(citychoice, 0, 47)) {
				// Move pawn to a proper city number
				// Now citychoice holds the node number to move to
				if (playerOfPawn == playerID - 1) {
					DiscardCardAndMove command(playerList[playerOfPawn], map->getCityList()->at(citychoice), choice, playerDeck);
					command.execute();
				}
				else {
					MovePawn command1(playerList[playerOfPawn], map->getCityList()->at(citychoice));
					command1.execute();
					DiscardCard command2(playerList[playerID - 1], &choice, 1, playerDeck);
					command2.execute();
				}
				cout << "Player " << playerOfPawn + 1 << " is now in " << playerList[playerOfPawn]->getPawn()->getName() << endl << endl;
				return true;
			}
		} // END AIRLIFT

		  // CASE: ONE QUIET NIGHT
		else if (playerList[playerID - 1]->checkifPlayerHasCardAtIndex(QUIET, choice)) {
			cout << "Performing the One Quiet Night event..." << endl;
			quietNightPlayed = true;
			cout << "A quiet night is near!" << endl;
			// Discard the card
			DiscardCard command(playerList[playerID - 1], &choice, 1, playerDeck);
			command.execute();
			return true;
		} // END ONE QUIET NIGHT

		  // CASE: FORECAST
		else if (playerList[playerID - 1]->checkifPlayerHasCardAtIndex(FORECAST, choice)) {
			cout << "Performing the Forecast event..." << endl;
			cout << "Here are the top 6 cards of the Infection Deck:\n" << endl;

			// To store to re-arrange later
			Card* infectCards[NUM_OF_CARDS_FOR_FORCAST];

			// Get the top 6 (back in deque) cards
			for (unsigned int i = 0; i < NUM_OF_CARDS_FOR_FORCAST; i++) {
				cout << i << ") " << infectionDeck->getDrawPile().back()->print() << endl;
				infectCards[i] = infectionDeck->getDrawPile().back();
				infectionDeck->getDrawPile().pop_back();
			}

			// Prompt for player input for order
			cout << "Please enter the cards in the order you wish to replace them. The last selection will be on the top"<< endl;
			int order[6];
			for (int i = 0; i < NUM_OF_CARDS_FOR_FORCAST; i++) {

				// Input Tick for visual purposes 
				cout << "> ";

				//Checks for valid input
				if (!getIntInput(order[i], 0, NUM_OF_CARDS_FOR_FORCAST)) {
					return false;
				}
			}

			// Arrange the deck in chosen order, and output as we go to let player know
			cout << "New top 6 cards are:\n" << endl;
			infectionDeck->getDrawPile().push_back(infectCards[order[0]]);
			cout << "0) " << infectionDeck->getDrawPile().back()->print() << endl;
			infectionDeck->getDrawPile().push_back(infectCards[order[1]]);
			cout << "1) " << infectionDeck->getDrawPile().back()->print() << endl;
			infectionDeck->getDrawPile().push_back(infectCards[order[2]]);
			cout << "2) " << infectionDeck->getDrawPile().back()->print() << endl;
			infectionDeck->getDrawPile().push_back(infectCards[order[3]]);
			cout << "3) " << infectionDeck->getDrawPile().back()->print() << endl;
			infectionDeck->getDrawPile().push_back(infectCards[order[4]]);
			cout << "4) " << infectionDeck->getDrawPile().back()->print() << endl;
			infectionDeck->getDrawPile().push_back(infectCards[order[5]]);
			cout << "5) " << infectionDeck->getDrawPile().back()->print() << endl;

			// Remove the card
			DiscardCard command(playerList[playerID - 1], &choice, 1, playerDeck);
			command.execute(); 
			return true;
		} // END FORECAST

		  // CASE: GOVERNMENT
		else if (playerList[playerID - 1]->checkifPlayerHasCardAtIndex(GOVERNMENT, choice)) {
			cout << "Performing the Government event..." << endl;
			cout << "City List: \n" << endl;
			// Output all city names
			cout << "\nPlease enter the number associated to a city you want to build a Research Station in." << endl;
			map->showAllCityOptions();
			// Validate city input
			int citychoice = 0;
			if (getIntInput(citychoice, 0, 47)) {
				//Check if there is no research station here
				CityNode* city = (map->getCityList()->at(citychoice));
				if (city->hasResearchStation()) {
					cout << "There is already a research station here." << endl;
					return false;
				}
				if (markers->useResearchStation()) {
					city->addResearchStation();
					// Remove the card (discard it)
					DiscardCard command(playerList[playerID - 1], &choice, 1, playerDeck);
					command.execute();
					cout << "Research Station Added! ";
					return true;
				}
				else {
					cout << "No more Research Stations to be built." << endl;
					cout << "This card has no purpose, it has be discarded." << endl;
					DiscardCard command(playerList[playerID - 1], &choice, 1, playerDeck);
					command.execute();
				}
			}
		} // END GOVERNMENT

		  // CASE: RESILIENT POPULATION
		else if (playerList[playerID - 1]->checkifPlayerHasCardAtIndex(RESILIENT, choice)) {
			cout << "Performing the Resilient Population event..." << endl;
			cout << "Infection Discard Pile: \n" << endl;
			// Print out infection discard pile
			for (unsigned int i = 0; i < infectionDeck->getDiscardPile().size(); i++) {
				cout << i << ") " << infectionDeck->getDiscardPile().at(i)->print() << endl;
			}
			cout << "\nPlease enter the number associated to the card you wish to remove from the game." << endl;
			// Read and validate input for the card choice to remove
			int card_choice = 0;
			if (!getIntInput(card_choice, 0, infectionDeck->getDiscardPile().size())) {
				return false;
			}
			else {
				// Remove the card from the game
				infectionDeck->getDiscardPile().erase(infectionDeck->getDiscardPile().begin() + card_choice);
				cout << "\nInfection Discard Pile is now made up of:\n" << endl;
				// Print out infection discard pile again to validate
				for (unsigned int i = 0; i < infectionDeck->getDiscardPile().size(); i++) {
					cout << i << ") " << infectionDeck->getDiscardPile().at(i)->print() << endl;
				}
				// Discard the event card
				DiscardCard command(playerList[playerID - 1], &choice, 1, playerDeck);
				command.execute();
				cout << "Your event card has been discarded" << endl;
				return true;
			}
		} // END RESILIENT POPULATION
		else {
			cout << "Your selection was not a valid Event Card." << endl;
		}
	}

	// Default if nothing completes successfully or if input is invalid
	return false;
}