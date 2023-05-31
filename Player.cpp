#include "Player.h"
#include <limits>
#include <climits>
using namespace std;

//base class
Player::Player(std::string name):m_name(name){}

Player::~Player() {}

std::string Player::name() const {
    return m_name;
}

bool Player::isInteractive() const {
    return false;
}

//human player
HumanPlayer::HumanPlayer(std::string name):Player(name) {}

bool HumanPlayer::isInteractive() const {
    return true;
}

int HumanPlayer::chooseMove(const Board &b, Side s) const {
    int choice = -1;
    do {
        cout << "Select a hole, " << name() + ": ";
        cin >> choice;
        if (choice <= 0 || choice > b.holes() ){
            cout << "The hole number must be from 1 to " << b.holes() << "." << endl;
        }
        else if (b.beans(s, choice) == 0) {
            cout << "There are no beans in that hole." << endl;
        }
    } while (choice <= 0 || choice > b.holes() || b.beans(s, choice) == 0);
    return choice;
}

//bad player
BadPlayer::BadPlayer(std::string name): Player(name) {}

//choose the first hole from the end with any beans in it
int BadPlayer::chooseMove(const Board &b, Side s) const{
    for (int i = b.holes(); i > 0; i--){
        if (b.beans(s, i) > 0){
            return i;
        }
    }
    return -1;
}

//SMART PLAYER
SmartPlayer::SmartPlayer(std::string name) : Player(name)
{
    
}

int SmartPlayer::chooseMove(const Board& b, Side s) const {
    int bestHole = -1;
    int value = 0;
    int depth = 0;
    recursiveChooseMove(b, s, bestHole, value, depth); //start recursive move selection
    return bestHole; //return best hole found
}

bool SmartPlayer::completeMove(Side s, int hole, Board& b, Side& endSide, int& endHole) const {
    if (b.sow(s, hole, endSide, endHole)) {
        if (endHole == 0) { //check if last bean ends up in pot of the same player
            return false; //if so, take another turn
        } else if (endSide == s) {
            if (b.beans(s, endHole) == 1 && b.beans(opponent(s), endHole) > 0) {
                b.moveToPot(opponent(s), endHole, s); //capture beans from opponent's hole
                b.moveToPot(s, endHole, s); //move player's last bean to their pot
            }
        }
        return true; //move is complete
    }
    return false; //invalid move
}

void SmartPlayer::recursiveChooseMove(const Board& b, Side s, int& bestHole, int& value, int depth) const {
    if (depth > 6) { //stop recursion if depth threshold is reached
        bestHole = -1;
        value = evaluate(b); //evaluate the state of the board
        return;
    }
    if (b.beansInPlay(s) == 0) { //stop recursion if current side runs out of beans
        bestHole = -1;
        value = evaluate(b); //evaluate the board state
        return;
    }
    int first = 1; //store first valid hole encountered
    for (int i = 1; i <= b.holes(); i++) {
        if (b.beans(s, i) == 0) {
            first++;
            continue; //skip the empty holes
        }
        Board temp(b); //create temporary board to simulate moves
        Side endSide;
        int endHole, v2, h2, h3;
        if (completeMove(s, i, temp, endSide, endHole) ) { //added and conditional
            recursiveChooseMove(temp, opponent(s), h2, v2, depth + 1); //recursive call for oponents turn
        } else {
            recursiveChooseMove(temp, s, h3, v2, depth); //recursive call for same players turn!
        }
        if (i == first) { //store value and hole of the first valid move encountered
            value = v2;
            bestHole = i;
        }
        if (s == SOUTH) {
            if (v2 > value) { //maximizing south player
                value = v2;
                bestHole = i;
            }
        } else {
            if (v2 < value) { //minimizing north player
                value = v2;
                bestHole = i;
            }
        }
    }
    return;
}

int SmartPlayer::evaluate(const Board& b) const {
    int north = b.beansInPlay(NORTH);
    int south = b.beansInPlay(SOUTH);
    int n_pot = b.beans(NORTH, 0);
    int s_pot = b.beans(SOUTH, 0);
    if (south == 0 || north == 0) {
        if (s_pot + south > n_pot + north) {
            return INT_MAX; //south wins
        } else if (s_pot + south < n_pot + north) {
            return INT_MIN; //north wins
        } else {
            return 0; //if no winner, draw
        }
    }
    return s_pot + south - n_pot - north; //the difference in bean counts
}
