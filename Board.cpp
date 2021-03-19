#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole){
    if(nHoles <= 0){
        mNumHoles = 1;
    }
    else{
        mNumHoles = nHoles;
    }
    if(nInitialBeansPerHole < 0){
        mBeansPerHole = 0;
    }
    else{
        mBeansPerHole = nInitialBeansPerHole;
    }
    /// implementation for using vector STL
    mNorthHoles.push_back(0);
    for(int i = 0; i < mNumHoles; i++){
        mNorthHoles.push_back(mBeansPerHole);
    }
    mSouthHoles.push_back(0);
    for(int i = 0; i < mNumHoles; i++){
        mSouthHoles.push_back(mBeansPerHole);
    }
}

int Board::holes() const{
    return mNumHoles;
}

int Board::beans(Side s, int hole) const{
    if(hole < 0 || hole > holes()) return -1;
    if(hole == POT){
        if(s == NORTH){
            return mNorthHoles[0];
        }
        if(s == SOUTH){
            return mSouthHoles[0];
        }
    }
    if(s == NORTH){
        return mNorthHoles[hole];
    }
    if(s == SOUTH){
        return mSouthHoles[hole];
    }
    return -1;
}
int Board::beansInPlay(Side s) const{
    if(s == NORTH){
        int northTotal = 0;
        for(int i = 1 ; i < mNorthHoles.size(); i++){
            northTotal += mNorthHoles[i];
        }
        return northTotal;
    }
    else{
        int southTotal = 0;
        for(int i = 1 ; i < mSouthHoles.size(); i++){
            southTotal += mSouthHoles[i];
        }
        return southTotal;
    }
}
int Board::totalBeans() const{
    int northTotal = 0;
    for(int i = 0; i < mNorthHoles.size(); i++){
        northTotal += mNorthHoles[i];
    }
    int southTotal = 0;
    for(int i = 0 ; i < mSouthHoles.size(); i++){
        southTotal += mSouthHoles[i];
    }
    return northTotal+southTotal;
}
bool Board::sow(Side s, int hole, Side& endSide, int& endHole){
   if(hole <= POT || hole > holes()){
        return false;
    }
    if(s == NORTH){ // implementation very similar for both NORTH and SOUTH
        if(mNorthHoles[hole] == 0) return false;
        int counter = mNorthHoles[hole];
        mNorthHoles[hole] = 0;
        endSide = NORTH;
        hole--;
        while(counter > 0){ // if there are still beans to sow
            while(hole != POT && counter > 0){ // traversing through the NORTH side first
                mNorthHoles[hole] += 1;
                endSide = NORTH;
                endHole = hole;
                hole--;
                counter--;
            }
            if(counter > 0){ // if bean added to POT
                counter--;
                mNorthHoles[0] += 1;
                endHole = hole;
                hole = 1; // set hole to the first possible position in the SOUTH side
            }
            while(hole <= holes()  && counter > 0){ // traversing through the SOUTH side
                mSouthHoles[hole] += 1;
                endSide = SOUTH;
                endHole = hole;
                hole++;
                counter--;
            }
            hole = holes(); // set hole to first possible position of NORTH side
        }
    }
    if(s == SOUTH){
        if(mSouthHoles[hole] == 0) return false;
        int counter = mSouthHoles[hole];
        mSouthHoles[hole] = 0;
        endSide = SOUTH;
        hole++;
        while(counter > 0){
            while(hole <= holes()  && counter > 0){
                mSouthHoles[hole] += 1;
                endSide = SOUTH;
                endHole = hole;
                hole++;
                counter--;
            }
            if(counter > 0){
                counter--;
                mSouthHoles[0] += 1;
                hole = holes();
                endHole = 0;
            }
            while(hole != POT && counter > 0){
                mNorthHoles[hole] += 1;
                endSide = NORTH;
                endHole = hole;
                hole--;
                counter--;
            }
            hole = 1;
        }
    }
    return true;
}
bool Board::moveToPot(Side s, int hole, Side potOwner){
    if(hole <= POT || hole > holes()) return false;
    if(s == NORTH){
        if(potOwner == NORTH){
            mNorthHoles[0] += mNorthHoles[hole];
            mNorthHoles[hole] = 0;
        }
        if(potOwner == SOUTH){
            mSouthHoles[0] += mNorthHoles[hole];
            mNorthHoles[hole] = 0;
        }
    }
    if(s == SOUTH){
        if(potOwner == NORTH){
            mNorthHoles[0] += mSouthHoles[hole];
            mSouthHoles[hole] = 0;
        }
        if(potOwner == SOUTH){
            mSouthHoles[0] += mSouthHoles[hole];
            mSouthHoles[hole] = 0;
        }
    }
    return true;
}
bool Board::setBeans(Side s, int hole, int beans){
    if(hole < POT || beans < 0 || hole > holes()) return false;
    if(s == NORTH){
        mNorthHoles[hole] = beans;
    }
    if(s == SOUTH){
        mSouthHoles[hole] = beans;
    }
    return true;
}

