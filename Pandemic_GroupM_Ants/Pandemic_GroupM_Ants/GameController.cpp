#include "GameController.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//----CONSTRUCTOR / DECONSTRUCTOR-----
////////////////////////////////////////////////////////////////////////////////////////////////////

GameController::GameController() : activePlayer(0), eventCardInPlay(false){
}

GameController::~GameController() {
	for (int i = 0; i < players.size(); i++) {
		delete players[i];
		players[i] = nullptr;
	}
	delete map;
	map = nullptr;
	delete infectionDeck;
	infectionDeck = nullptr;
	delete playerDeck;
	playerDeck = nullptr;

	// OBSERVER PATTERN: Delete MapView and PlayerView
	delete mapDisplay;
	mapDisplay = nullptr;

	delete playerDisplay;
	playerDisplay = nullptr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// -----INITIALIZE--------
/////////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize New Game: Creates all the base member for the game
void GameController::initializeNewGame()
{
	//initialize models
	map = new GameMap(MAP_INITIAL_FILE);
	infectionDeck = new Deck(INFECTION_CARD_INITIAL_FILE);
	//infectionDeck = DeckFactory::Create(INFECTION_CARD_INITIAL_FILE);
	playerDeck = new Deck(PLAYER_CARD_INITIAL_FILE);
	//playerDeck = DeckFactory::Create(PLAYER_CARD_INITIAL_FILE);
	
	//Setup starting conditions
	map->getHead()->addResearchStation();
	markers.useResearchStation();

	initializePlayers();
	addEpidemicCards();
	initialInfection();
	

	initializeObservers(); //handles the registration and creation of all views

	pause();
}

// Initialize Players: Initialize n number of players
void GameController::initializePlayers() 
{
	cout << "\nHow many players? (2, 3 or 4?)" << endl;

	// Input Tick for visual purposes 
	cout << "> ";

	while (!getIntInput(numOfPlayers, 2, 4)) {}

	Deck* roleDeck = new Deck(ROLE_CARD_INITIAL_FILE);
	//Deck* roleDeck = DeckFactory::Create(ROLE_CARD_INITIAL_FILE);
	for (int i = 0; i < numOfPlayers; i++) {
		players.push_back(new Player(i, new ReferenceCard()));
		players[i]->movePawn(map->getHead());
		players[i]->setRole(dynamic_cast<RoleCard*>(roleDeck->drawFromTop()));
		
		//Deal starting cards.
		int num_of_starting_cards_per_player = 6 - numOfPlayers;
		for (int j = 0; j < num_of_starting_cards_per_player; j++) {
			players[i]->addCard(playerDeck->drawFromTop());
		}
	}
	delete roleDeck;
}

//Adds Epidemic cards to player deck
void GameController::addEpidemicCards()
{
	int selection;
	cout << "\nWhat will be the difficulty of the game? " << endl;
	cout << "1) Introductory" << endl;
	cout << "2) Standard" << endl;
	cout << "3) Heroic" << endl;

	// Input Tick for visual purposes 
	cout << "> ";

	//Handle input
	while(!getIntInput(selection, 1, 3)){}
	playerDeck->insertEpidemicCards(NUM_OF_EPIDEMIC_CARDS + selection);
}

// Initial Infection: infect cities on new board game
void GameController::initialInfection() 
{
	for (int k = INITIAL_INFECTION; k > 0; k--) {
		for (int i = 0; i < INITIAL_INFECTION; i++) {
			InfectionCard* card = dynamic_cast<InfectionCard*>(infectionDeck->drawFromTop());
			for (int j = 0; j < k; j++) {
				markers.useDiseaseCube(card->getColor());
				map->getCity(card->getCity())->increaseInfectionLevel(card->getColor());
				cout << "A " << infectTypeEnumToString(card->getColor()) << " cube has been added to " << card->getCity() << endl;
			}
			infectionDeck->discard(card);
		}
	}
}

void GameController::initializeObservers() 
{
	//new MapView
	mapDisplay = new MapView(&markers, map, players);

	//new PlayerView
	playerDisplay = new PlayerView(&players);

	for (auto const &player : players) {
		medicObserver = new MedicObserver(player, map, &markers);
	}

	//initialize numberOfPlayers
	numOfPlayers = players.size();

	checkifEventCardinPlay();
}

//Check if any Player has an event card; toggles event card play on and off
void GameController::checkifEventCardinPlay()
{
	eventCardInPlay = false;
	for (int i = 0; i < players.size(); i++) {
		if (players[i]->checkifPlayerHasEventCard()) {
			eventCardInPlay = true;
		}
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////
// -----SAVE/LOAD--------
/////////////////////////////////////////////////////////////////////////////////////////////////////

// Load game via Serialization
void GameController::loadGame()
{ 
	cout << "Restoring saved game..." << endl;
	// Create and input archive
	std::ifstream ifs(SAVED_MAP_FILE);
	boost::archive::text_iarchive ar(ifs);
	ar & markers & map & infectionDeck & playerDeck & players;
	ifs.close();

	initializeObservers(); //handles the registration and creation of all views
}

// Same game via Serialization
void GameController::saveGame()
{
	cout << "Saving game..." << endl;
	// Create and input archive
	std::ofstream ofs(SAVED_MAP_FILE);
	boost::archive::text_oarchive ar(ofs);
	ar & markers & map & infectionDeck & playerDeck & players;
	ofs.close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// -----GAME ROUND--------
/////////////////////////////////////////////////////////////////////////////////////////////////////

void GameController::startRound()
{
	for (int i = 0; i < players.size(); i++) {
		quietNightPlayed = false;
		//Declare player order
		system("cls");
		cout << "Player " << activePlayer + 1 << "'s turn starts now." << endl << endl;
		pause();

		phase1_Actions();	  // Do Phase 1: Player 4 turns
		phase2_DrawCards();	  // Do Phase 2: Draw 2 cards
		phase3_Infects();	  // Do Phase 3: Infect the cities
		switchPlayers();
	}
	system("cls");

	// Updates the map and display all models
	// mapDisplay->updateMap(map->getCityList());
	// mapDisplay->printMap();	// Display Map Views
	mapDisplay->Update("print");	// Display Map Views

	for (int i = 0; i < players.size(); i++) {
		playerDisplay->setActivePlayer(i);
		playerDisplay->Update("print");
	}
}

// Switch Player Function
void GameController::switchPlayers()
{	activePlayer = (activePlayer + 1) % players.size();	}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// -----PHASE 1--------
/////////////////////////////////////////////////////////////////////////////////////////////////////

void GameController::phase1_Actions()
{
	// Set the PlayerView to the current active player
	mapDisplay->setActivePlayer(activePlayer);
	playerDisplay->setActivePlayer(activePlayer);

	int i = 0;
	while(i < NUM_OF_ACTIONS) {

		//==================== CLEAR SCREEN ====================
		system("cls");

		//==================== DISPLAY VIEW ====================

		// Display Map Views
		mapDisplay->Update("print");	

		// Display player view
		playerDisplay->Update("print");


		/*cout << "=============================================================" << endl;
		cout << "------------------------Phase 1------------------------------" << endl;
		cout << "=============================================================" << endl << endl;*/

		ActionContext action;

		//display actions
		int choice = 0;
		cout << "You have " << (NUM_OF_ACTIONS - i) << " actions left. \nWhich would you like to perform?\n" << endl;
		cout << "(1) Drive OR Ferry               (6) Discover A Cure" << endl;
		cout << "(2) Direct Flight                (7) Treat A Disease" << endl;
		cout << "(3) Charter Flight               (8) Share Knowledge" << endl;
		cout << "(4) Shuttle Flight               (9) Show Reference Card [ NOTE: This does not count as an action ]" << endl;
		cout << "(5) Build a Research Station     ";

		/*cout << "(6) Discover A Cure" << endl;
		cout << "(7) Treat A Disease" << endl;
		cout << "(8) Share Knowledge" << endl;
		cout << "(9) Show Reference Card [NOTE: This does not count as an action!]" << endl;*/
		if (*players[activePlayer] == DISPATCHER) {
			cout << "(10) Dispatcher Movement: Move a pawn to another pawn." << endl;
		}
		if (*players[activePlayer] == PLANNER) {
			cout << "(11) Contingency Planner: Take an Event card from the discard pile" << endl;
		}
		cout << "(12) Stop" << endl;

		// Input Tick for visual purposes 
		cout << "\n> ";
		
		//process input of actions
		while(!getIntInput(choice, 1, 12)){}
		cout << endl;
		cout << "=======================================================" << endl;

		switch (choice)
		{
		case 1:
			action.setStrategy(new DriveFerry());
			break;
		case 2:
			action.setStrategy(new DirectFlight());
			break;
		case 3:
			action.setStrategy(new CharterFlight());
			break;
		case 4: 
			action.setStrategy(new ShuttleFlight());
			break;
		case 5:
			action.setStrategy(new BuildResearchStation());
			break;
		case 6:
			action.setStrategy(new DiscoverCure());
			break;
		case 7:
			action.setStrategy(new TreatDisease());
			break;
		case 8:
			action.setStrategy(new ShareKnowledge());
			break;
		case 9:
			action.setStrategy(new ShowRefCard());
			break;
		case 10:
			action.setStrategy(new DispatcherMovement());
			break;
		case 11:
			action.setStrategy(new TakeEventCard());
			break;
		// Skip turn
		case 12: 
			cout << "Ending Actions Turn" << endl;
			action.setStrategy(new DoNothing());
			i = 4;
			break;
		// Error if action is invalid
		default:
			cout << "Something went wrong, no valid action selected." << endl;
			break;
		}
		if (action.execute(map, &markers, players, playerDeck, infectionDeck, activePlayer, quietNightPlayed)) {
			i++;
		}
		
		//If any player has an event card then prompt
		if (eventCardInPlay) {
			int playerID = 0;
			// Prompt for event cards
			if (promptEvent(playerID, numOfPlayers)) {
				ActionContext action(new DoEventCards());
				action.execute(map, &markers, players, playerDeck, infectionDeck, playerID, quietNightPlayed);
			}
		}

		checkifEventCardinPlay();

		//Check if win conditions have been met
		if (markers.getCureMarker(yellow) == cured && markers.getCureMarker(red) == cured && markers.getCureMarker(blue) == cured && markers.getCureMarker(black) == cured) {
			//WIN THE GAME
			endGame(true, "You Have Cured All Four Diseases!");
		}

		// Pause...
		pause();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// -----PHASE 2--------
/////////////////////////////////////////////////////////////////////////////////////////////////////

void GameController::phase2_DrawCards()
{
	system("cls");
	cout << "=============================================================" << endl;
	cout << "------------------------Phase 2------------------------------" << endl;
	cout << "=============================================================" << endl << endl;
	cout << "Player " << activePlayer + 1 << " draws two cards" << endl;
	
	//Check how many cards left in draw pile
	if (playerDeck->cardsInDeck() < 2) {
		// GAME OVER
		endGame(false, "No More Player Cards Left!");
	}

	else {
		//Draw two cards
		ActionContext action(new DrawCard());
		for (int i = 0; i < NUM_OF_PLAYER_CARDS_DRAWN_PER_TURN; i++) {
			action.execute(map, &markers, players, playerDeck, infectionDeck, activePlayer, quietNightPlayed);
			if (players[activePlayer]->checkifPlayerHasEventCard()) {
				eventCardInPlay = true;
			}
		}
	}

	//If any player has an event card, then prompt
	if (eventCardInPlay) {
		int playerID = 0;
		// Prompt for event cards
		if (promptEvent(playerID, numOfPlayers)) {
			ActionContext action(new DoEventCards());
			action.execute(map, &markers, players, playerDeck, infectionDeck, playerID, quietNightPlayed);
			checkifEventCardinPlay();
		}
	}

	pause();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// -----PHASE 3--------
/////////////////////////////////////////////////////////////////////////////////////////////////////

//  Phase 3: End Turn Infection 
void GameController::phase3_Infects()
{
	system("cls");
	cout << "=============================================================" << endl;
	cout << "------------------------Phase 3------------------------------" << endl;
	cout << "=============================================================" << endl << endl;
	
	if (!quietNightPlayed) {
		//Draw Cards and infect cities
		cout << "Player " << activePlayer + 1 << " draws " << markers.getInfectionRate() << " infection cards." << endl << endl;
		ActionContext action(new Infect());
		for (int i = 0; i < markers.getInfectionRate(); i++) {
			action.execute(map, &markers, players, playerDeck, infectionDeck, activePlayer, quietNightPlayed);
		}
		if (markers.getOutbreakCounter() == MAX_OUTBREAKES) {
			//game over
			endGame(false, "You have reach Outbreak Level 8 (Crossbone Marker)!");
		}
		//check if all cubes of a color have been used.
		for (int i = 0; i < NUM_OF_DISEASES; i++) {
			InfectType infection = static_cast<InfectType>(i);
			if (markers.getNumOfDiseaseCubes(infection) == 0) {
				//game over
				endGame(false, "No More " + infectTypeEnumToString(infection) + " Disease Cubes Left!");
			}
		}
	}
	else {
		cout << "You had a quiet night, no new disease cubes were placed on the board." << endl;
	}

	pause();
}

// End Game Screen (Winning and Losing Scenario)
void GameController::endGame(bool isWon, string message) {

	system("CLS");

	if (isWon) {
		cout << " ================================================================== " << endl;
		cout << "                                YOU WIN!" << endl;
		cout << " ================================================================== \n" << endl;
		cout << message << "\n" << endl;
	
		pause();
		exit(0);
	}
	else {
		cout << " ================================================================== " << endl;
		cout << "                               YOU LOSE!" << endl;
		cout << " ================================================================== \n" << endl;
		cout << "Reason: " << message << "\n" << endl;

		pause();
		exit(0);
	}
}

