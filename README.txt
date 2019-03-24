-----------------------------
| COMP345 - PROJECT BUILD #1| 
|			    |
| PROGRAMMED BY:	    |
| Philip Michael 	    |
| Amanda Friesen 	    |
| Johnny Mak	     	    |
| Jason Kalec     	    |
|			    |
| PRESENTED TO:		    |
| Prof. Nora Houari	    |
-----------------------------

	=== GROUP: ANTS ===

(For additional in depth information, please refer to the provided project report document.)

// 1 \\ - PROJECT DESCRIPTION
This C++ project implements a functioning rendition of the popular board game: Pandemic!
Fight off terrible diseases by discovering cures in your quest to save the world!
The game functions without a Graphical User Interface, so all necessary information is presented via console output/input.
Read on for specific information about the implementation and design.

// 2 \\ - PROJECT IMPLEMENTATION AND DESIGN
The design for the project was heavily centered on functionality and ease of use.
Using console input eliminates ambiguity and allows for clear and concise operational interaction for the game.
The players are given their respective turns, and they can perform their desired actions in any order they like.
Finally, turn functionality is present and the game play does indeed function based on the game's original rules.
Modules are cohesive and follow a sort of logical sequence. Filters are present in the sln file to reflect that.
The MVC architecture was used heavily throughout the project.

// 3 \\ - BASIC GAME PLAY
Upon launching the application, the user is presented with 2 choices: [1] start a new game; or [2] load a saved game.
If the user selects to start a new game, the engine immediately constructs all necessary elements for the game.
Once all the setup is complete (it should take a few seconds at most), the map (game board) is displayed and actions can be performed.
Initially, the engine will also display the current player's information which includes the role associated to the player (each role is unique).
It will also output the role's unique description as well as the player's current hand.
Finally, the actions options are outputted and the user is to select one to perform.
Each player can perform up to 4 actions per turn, and then the turns alternate.
When the turn swaps over, the previous turn player draws 2 cards and we alert the player of the drawn cards then add them to their hand.
And that concludes basic game play.


We hope you enjoy our version of Pandemic!
(The best version ;) )
