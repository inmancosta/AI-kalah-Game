#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole) {
    if (nHoles < 1) {
        nHoles = 1;
    }
    m_holes = nHoles + 1; // Pot is stored as a hole

    if (nInitialBeansPerHole < 0) {
        nInitialBeansPerHole = 0;
    }

    totalBeansCount = 2 * (m_holes - 1) * nInitialBeansPerHole; // Total number of beans in the game
    initializeVector(nInitialBeansPerHole, northHoles); // Initialize the vector for North's holes
    initializeVector(nInitialBeansPerHole, southHoles); // Initialize the vector for South's holes
}

int Board::holes() const {
    // Return the number of holes on a side (not counting the pot).
    return m_holes - 1;
}

int Board::beans(Side s, int hole) const {
    if (!isValidHole(hole) || !isValidSide(s)) {
        return -1; // Invalid hole or side, return -1
    }

    if (s == SOUTH) {
        return southHoles[hole]; // Return the number of beans in the specified hole for South
    } else {
        return northHoles[hole]; // Return the number of beans in the specified hole for North
    }
}

int Board::beansInPlay(Side s) const {
    // Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
    if (!isValidSide(s)) {
        return -1; // Invalid side, return -1
    }

    int beansCount = 0;

    if (s == SOUTH) {
        for (int i = 1; i < southHoles.size(); i++) {
            beansCount += southHoles[i]; // Count the beans in South's holes
        }
    } else {
        for (int i = 1; i < northHoles.size(); i++) {
            beansCount += northHoles[i]; // Count the beans in North's holes
        }
    }

    return beansCount;
}

int Board::totalBeans() const {
    // Return the total number of beans in the game, including any in the pots.
    return totalBeansCount;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {
    if (!isValidHole(hole)) {
        return false; // Invalid hole, return false
    }

    int count = 0;

    if (s == NORTH) { // Sowing on the north side
        count = northHoles[hole]; // Get the number of beans in the selected hole
        northHoles[hole] = 0; // Remove all beans from the selected hole

        if (count == 0) {
            endHole = hole;
            return false; // Empty hole, return false
        }

        int pos = hole - 1; // Start sowing from the previous hole

        while (count != 0) {
            for (int i = pos; i >= 0 && count != 0; i--) {
                northHoles[i]++; // Sow one bean in each hole towards the left
                count--; // Decrement the count of remaining beans
                if (count == 0) {
                    endSide = NORTH;
                    endHole = i;
                    return true; // Reached the end, return true
                }
            }

            pos = m_holes - 1; // Reset position to the last hole on the north side

            for (int i = 1; i < southHoles.size(); i++) {
                southHoles[i]++; // Sow one bean in each hole on the south side
                count--;
                if (count == 0) {
                    endSide = SOUTH;
                    endHole = i;
                    return true; // Reached the end, return true
                }
            }
        }

        return true; // Sowing complete
    } else { // Sowing on the south side
        count = southHoles[hole]; // Get the number of beans in the selected hole
        southHoles[hole] = 0; // Remove all beans from the selected hole

        if (count == 0) {
            endHole = hole;
            return false; // Empty hole, return false
        }

        int pos = hole + 1; // Start sowing from the next hole

        while (count != 0) {
            for (int i = pos; i < southHoles.size() && count != 0; i++) {
                southHoles[i]++; // Sow one bean in each hole towards the right
                count--;
                if (count == 0) {
                    endSide = SOUTH;
                    endHole = i;
                    return true; // Reached the end, return true
                }
            }

            pos = 1; // Reset position to the first hole on the south side

            if (count != 0) {
                southHoles[0]++; // Sow one bean in the store (pot)
                count--;
            }

            if (count == 0) {
                endSide = SOUTH;
                endHole = 0;
                return true; // Reached the end, return true
            }

            for (int i = m_holes - 1; i > 0; i--) {
                northHoles[i]++; // Sow one bean in each hole on the north side
                count--;
                if (count == 0) {
                    endSide = NORTH;
                    endHole = i;
                    return true; // Reached the end, return true
                }
            }
        }

        return true; // Sowing complete
    }
}

bool Board::moveToPot(Side s, int hole, Side potOwner) {
    if (!isValidHole(hole) || !isValidSide(s) || hole == 0) {
        return false; // Invalid hole or side, or trying to move to an invalid pot, return false
    }

    // Move beans from the current hole to the pot
    if (s == SOUTH) {
        if (potOwner == NORTH) {
            northHoles[POT] += southHoles[hole];
        } else {
            southHoles[POT] += southHoles[hole];
        }
        southHoles[hole] = 0; //empty the hole
    } else {
        if (potOwner == NORTH) {
            northHoles[POT] += northHoles[hole];
        } else {
            southHoles[POT] += northHoles[hole];
        }
        northHoles[hole] = 0; //empty the hole
    }

    return true;
}

bool Board::setBeans(Side s, int hole, int beans) {
    if (!isValidHole(hole) || !isValidSide(s) || beans < 0) {
        return false; // Invalid hole, side, or negative beans, return false
    }

    int prev;

    if (s == SOUTH) {
        prev = southHoles[hole]; //record original #of beans
        southHoles[hole] = beans;//reset value at that index
    } else {
        prev = northHoles[hole]; //record original #of beans
        northHoles[hole] = beans; //reset value at index
    }

    totalBeansCount = totalBeansCount - prev + beans; //update total bean count
    return true;
}

bool Board::isValidHole(int hole) const {
    for (int i = 0; i < m_holes; i++) { //check if hole number is in range of existing holes
        if (hole == i) {
            return true;
        }
    }
    return false;
}

bool Board::isValidSide(Side s) const {
    switch (s) {
    case NORTH: //must be north or south to be valid side,
    case SOUTH:
        return true;
    default: //otherwise false
        return false;
    }
}

void Board::initializeVector(int beans, std::vector<int>& m_vector) {
    m_vector.push_back(0); // Pot value
    for (int i = 1; i < m_holes; i++) {
        m_vector.push_back(beans); // Initialize the remaining holes with the specified number of beans
    }
}
