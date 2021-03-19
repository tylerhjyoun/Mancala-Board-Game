#ifndef Player_h
#define Player_h

#include "Board.h"
#include "Side.h"
#include <string>
#include <iostream>

class Player {
public:
    Player(std::string name);
    std::string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player();
private:
    std::string mName;
};

class HumanPlayer: public Player {
public:
    HumanPlayer(std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
    virtual ~HumanPlayer();
};

class BadPlayer: public Player {
public:
    BadPlayer(std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
    virtual ~BadPlayer();
};

class SmartPlayer: public Player {
public:
    SmartPlayer(std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
    virtual ~SmartPlayer();
private:
    const int max = 999;
    const int min = -999;
    Side s = SOUTH;
    int eval(Board b, Side s) const;
    void chooseBestMove(Board b, Side s,int& bestHole, int& value, int depth) const;
};

#endif

