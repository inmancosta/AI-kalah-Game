#include "Game.h"
using namespace std;

Game::Game(const Board& b, Player* south, Player* north) :m_board(b), m_north(north), m_south(south), turn(SOUTH) {}

void Game::display() const
{
    int numHoles = m_board.holes();
    std::cout << "\t "<< m_north->name() << std::endl << "    "; // outputs north's name
    for(int i = 1; i <= numHoles; i++) // outputs the north holes
    {
        std::cout << m_board.beans(NORTH, i) << "  ";
    }
    std::cout << std::endl << "  " << m_board.beans(NORTH, 0); // outputs north's pot
    for(int i = 0; i < numHoles; i++) // outputs the spaces
    {
        std::cout<<"   ";
    }
    std::cout << m_board.beans(SOUTH, 0) << std::endl << "    "; // outputs south's pot
    for(int i = 1; i <= numHoles; i++) // outputs south's holes
    {
        std::cout << m_board.beans(SOUTH, i) << "  ";
    }
    std::cout << std::endl;
    std::cout << "\t " << m_south->name() << std::endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const {
    int northScore = m_board.beans(NORTH, 0) + m_board.beansInPlay(NORTH);
    int southScore = m_board.beans(SOUTH, 0) + m_board.beansInPlay(SOUTH);
    
    if (m_board.beansInPlay(NORTH) > 0 && m_board.beansInPlay(SOUTH) > 0){
        over = false;
        return;
    }
    
    if (northScore > southScore){ //north wins
        over = true;
        hasWinner = true;
        winner = NORTH;
        return;
    }
    if (southScore > northScore){ //south wins
        over = true;
        hasWinner = true;
        winner = SOUTH;
        return;
    }
   
    if(northScore == southScore){ //tie
        over = true;
        hasWinner = false;//no winner if tied!
        return;
    }
}
bool Game::move(Side s) {
    int endHole;
    Side endSide;

    int hole;
    if (s == SOUTH) {
        hole = m_south->chooseMove(m_board, s);
        if (!m_south->isInteractive()) {
            cout << m_south->name() + " chooses hole " << hole << endl;
        }
    } else {
        hole = m_north->chooseMove(m_board, s);
        if (!m_north->isInteractive()) {
            cout << m_north->name() + " chooses hole " << hole << endl;
        }
    }

    // Sow the beans and handle the game rules
    if (m_board.sow(s, hole, endSide, endHole)) {
        if (endHole == POT) {
            display(); // Display the current state of the game board
            if (m_board.beansInPlay(s) > 0) {
                if (s == SOUTH) {
                    cout << m_south->name() << " gets another turn." << endl;
                } else {
                    cout << m_north->name() << " gets another turn." << endl;
                }
                return move(s); // The player gets another turn
            }
        } else if (endSide == s) {
            if (m_board.beans(s, endHole) == 1 && m_board.beans(opponent(s), endHole) > 0) {
                // Capture the beans on both sides and move them to the respective pots
                m_board.moveToPot(opponent(s), endHole, s);
                m_board.moveToPot(s, endHole, s);
            }
        }
    }

    // Switch the turn to the opponent
    turn = opponent(turn);

    // Check if the game is over and handle the end game scenario
    if (m_board.beansInPlay(NORTH) == 0) { // Game is over, South has no beans left
        display();
        cout << "Sweeping remaining beans into " + m_south->name() + "'s pot." << endl;
        for (int i = 1; i <= m_board.holes(); i++) {
            m_board.moveToPot(SOUTH, i, SOUTH);
        }
        return false; // Return false to indicate the game is over
    } else if (m_board.beansInPlay(SOUTH) == 0) { // Game is over, North has no beans left
        display();
        cout << "Sweeping remaining beans into " + m_north->name() + "'s pot." << endl;
        for (int i = 1; i <= m_board.holes(); i++) {
            m_board.moveToPot(NORTH, i, NORTH);
        }
        return false; // Return false to indicate the game is over
    }

    return true; // Return true to continue the game
}

void Game::play() {
    bool isOver = false;
    bool winnerExists = false;
    Side winner = SOUTH;

    display(); // Display the initial game board

    while (!isOver) {
        move(turn); // Make a move based on the current turn
        display(); // Display the updated game board

        // Check if the game is interactive and wait for user input
        if (!m_south->isInteractive() && !m_north->isInteractive()) {
            cout << "Press ENTER to continue."; //press enter to advance moves
            cin.ignore(10000, '\n');
        }

        status(isOver, winnerExists, winner); // Update the game status

    }

    // Print the winner or declare a tie if no winner exists
    if (winnerExists) {
        if (winner == SOUTH) {
            cout << "The winner is " << m_south->name() << endl;
        } else {
            cout << "The winner is " << m_north->name() << endl;
        }
    } else {
        cout << "It's a tie." << endl;
    }
}

int Game::beans(Side s, int hole) const {
    return m_board.beans(s, hole); // Return the number of beans in a specified hole
}
