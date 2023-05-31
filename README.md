# AI-kalah-Game

Kalah is a traditional African and Asian game that has been played for centuries. This project aims to create a program that plays the game of Kalah effectively. The game involves two players, North and South, who take turns making moves on a board with multiple holes and pots. The objective is to collect the most beans in your pot by the end of the game.

<h2> Rules </h2>

The game board consists of two rows of holes, each row representing a player's side.
Each player has six holes on their side of the board, numbered from 1 to 6, and their own pot (kalah).
Initially, each hole contains four beans.
Players take turns making moves, starting with the South player.
On their turn, a player picks up all the beans from one of their holes and distributes them counterclockwise, placing one bean in each hole and their own pot (skipping the opponent's pot if necessary).
If the last bean lands in the player's pot, they get another turn.
If the last bean lands in an empty hole on the player's side and the opponent's corresponding hole is not empty, the player captures all the beans from both holes and places them in their pot.
The game ends when a player cannot make a move because all their holes are empty.
At the end of the game, any remaining beans in the opponent's holes are placed in their pot.
The player with the most beans in their pot wins the game.

<h2>Getting Started</h2>

To run the Kalah game, you need to compile and execute the program. The program consists of several classes that work together to play the game. The main classes are:

Board: Represents the game board and provides methods to interact with the board.
Player: An abstract base class for different types of players (human and computer players).
HumanPlayer: A class that represents a human player and allows them to make moves interactively.
BadPlayer: A simple computer player that makes arbitrary moves.
SmartPlayer: A computer player that uses an intelligent strategy to choose moves.
Game: Manages the game, including the board, players, and game flow.
To start the game, create instances of the desired players (human or computer) and a Board object. Then, create a Game object with the board and players. Finally, call the play() method on the Game object to start the game.



<h2>HumanPlayer</h2>
The HumanPlayer class represents a human player. It allows the player to interactively choose a move during the game. It derives from the Player class and overrides the chooseMove method.

<h2>BadPlayer</h2>
The BadPlayer class represents a computer player that makes arbitrary moves. It derives from the Player class and overrides the chooseMove method to choose a random valid move.

<h2>SmartPlayer</h2>
The SmartPlayer class represents a computer player that uses an intelligent strategy to choose moves. It derives from the Player class and overrides the chooseMove method. The implementation of the intelligent strategy should ensure that the move is returned within a reasonable time limit.


To run the Kalah game, compile the source code files and execute the resulting program. Make sure to include all the necessary class files. Once the program is running, follow the prompts to make moves during the game.

Example Usage

Here's an example of how to run a game with a human player (Marge) against a bad computer player (Homer) on a 3-hole board:


Copy code <br>
#include "Board.h" <br>
#include "Game.h" <br>
#include "HumanPlayer.h" <br>
#include "BadPlayer.h" <br>

int main() <br>
{ <br>
    HumanPlayer hp("Marge"); <br>
    BadPlayer bp("Homer"); <br>
    Board b(3, 2); <br>
    Game g(b, &hp, &bp); <br>
    g.play(); <br>
    return 0; <br>
}
