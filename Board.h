#ifndef Board_h
#define Board_h

#include <iostream>
#include "Side.h"
#include <vector>
using namespace std;

class Board {
public:
    Board(int nHoles, int nInitialBeansPerHole);
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
private:
    int mNumHoles;
    int mBeansPerHole;
    vector<int> mNorthHoles;
    vector<int> mSouthHoles;
};

#endif 
