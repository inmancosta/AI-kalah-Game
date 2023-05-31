#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;
int main()
    {
        HumanPlayer bp1("Bart");
        SmartPlayer bp2("Homer");
        Board b(4, 3);
        Game g(b, &bp1, &bp2);
        g.play();
    
    }
