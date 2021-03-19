#ifndef Game_h
#define Game_h

#include "Side.h"
#include "Player.h"
#include "Game.h"
#include <iostream>
class Game {
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move();
    void play();
    int beans(Side s, int hole) const;
private:
    Board mBoard;
    Player* mSouthPlayer;
    Player* mNorthPlayer;
    Side currentSide;
};

#endif
