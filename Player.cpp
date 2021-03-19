#include "Player.h"

//////////////////////////////////////////////////////////////
//PLAYER CLASS IMPLEMENTATIONS
//////////////////////////////////////////////////////////////
Player::Player(std::string name){
    mName = name;
}
std::string Player::name() const{
    return mName;
}
bool Player::isInteractive() const{
    return false;
}
Player::~Player(){}

//////////////////////////////////////////////////////////////
//HUMAN PLAYER CLASS IMPLEMENTATIONS
//////////////////////////////////////////////////////////////
HumanPlayer::HumanPlayer(std::string name):Player(name){
    
}
bool HumanPlayer::isInteractive() const{
    return true;
}
int HumanPlayer::chooseMove(const Board& b, Side s) const{
    if(isInteractive()){
        if(b.beansInPlay(s) == 0 || b.beansInPlay(opponent(s)) == 0) return -1;
        cout << "Select a move, " << Player::name() << ": ";
        int hole;
        cin >> hole;
        while(b.beans(s, hole) == 0 || hole <= POT || hole > b.holes()){
            if(hole <= POT || hole > b.holes()){
               cout << "The hole number must be from 1 to " << b.holes() << endl;
            }
            if(b.beans(s, hole) == 0){
                cout << "There are no beans in that hole." << endl;
            }
            cout << "Select a hole, " << name() << ": ";
            cin >> hole;
        }
        return hole;
    }
    return -1;
}
HumanPlayer::~HumanPlayer(){
}

//////////////////////////////////////////////////////////////
//BAD PLAYER CLASS IMPLEMENTATIONS
//////////////////////////////////////////////////////////////
BadPlayer::BadPlayer(std::string name):Player(name){
}
bool BadPlayer::isInteractive() const{
    return false;
}
int BadPlayer::chooseMove(const Board& b, Side s) const{
    int hole = 1; // choose the any hole that has beans in it starting from the first hole
    if(b.beansInPlay(s) == 0 || b.beansInPlay(opponent(s)) == 0) return -1;
    while(b.beans(s,hole) == 0 && hole < b.holes()){ // if no beans, go to the next hole
        hole++;
    }
    return hole;
    
}
BadPlayer::~BadPlayer(){}

//////////////////////////////////////////////////////////////
//SMART PLAYER CLASS IMPLEMENTATIONS
//////////////////////////////////////////////////////////////
SmartPlayer::SmartPlayer(std::string name):Player(name){
}
bool SmartPlayer::isInteractive() const{
    return false;
}
int SmartPlayer::chooseMove(const Board& b, Side s) const{
    int value;
    int hole = 1;
    if(b.beansInPlay(s) == 0 || b.beansInPlay(opponent(s)) == 0){
        return -1;
    }
    while(b.beans(s,hole) == 0 && hole < b.holes()){
        hole++;
    }
    chooseBestMove(b, s, hole, value, 6);
    return hole;
}
SmartPlayer::~SmartPlayer(){}


void SmartPlayer::chooseBestMove(Board b, Side s,int& bestHole, int& value, int depth) const{
    if(b.beansInPlay(s) == 0 || b.beansInPlay(opponent(s)) == 0){
        bestHole = -1;
        if((b.beans(s, 0)+b.beansInPlay(s)) > (b.beans(opponent(s), 0) + b.beansInPlay(opponent(s)))){
            value = 999 - depth; // want the value to be returned in as few moves as possible so the less moves, the greater the value
        }
        if((b.beans(s, 0)+b.beansInPlay(s)) < (b.beans(opponent(s), 0) + b.beansInPlay(opponent(s)))){
            value = -999 + depth; // the less moves played, the smaller the value
        }
        if((b.beans(s, 0)+b.beansInPlay(s)) == (b.beans(opponent(s), 0) + b.beansInPlay(opponent(s)))){
            value = 0;
        }
        return;
    }
    if(depth == 0){
        bestHole = -1;
        value = 2*(b.beans(s, 0) - b.beans(opponent(s), 0)) + (b.beansInPlay(s) - b.beansInPlay(opponent(s))); // heuristic that weights the difference in pots more than difference in beans in play
        return;
    }
    for(int i = 1; i <= b.holes(); i++){
        if(b.beans(s, i) != 0){
            Side es;
            int eh;
            int h2;
            int v2 = -99999;
            Board possibleBoard(b); // copy of board
            possibleBoard.sow(s, i, es, eh); // "make move"
            if (eh == POT){ // additional turn
                chooseBestMove(possibleBoard, s, bestHole, v2, depth); // recursive call
                if(value < v2){ // mini-max algorithm to return best possible move of all possible positions when taking additional turns
                    value = v2;
                    bestHole = i;
                }
            }
            else{
                if(es == s && possibleBoard.beans(s, eh) == 1 && possibleBoard.beans(opponent(s), eh) != 0 && eh != 0){ // accounting for capture possibility
                    possibleBoard.moveToPot(opponent(s), eh, s);
                    possibleBoard.moveToPot(s, eh, s);
                }
                chooseBestMove(possibleBoard, opponent(s), h2, v2, depth-1);
                if(value < -v2){ // mini-max algorithm to return best possible move so that the opponents possible values are minimized
                    value = -v2;
                    bestHole = i;
                }
            }
            
        }
    }
    return;
}



