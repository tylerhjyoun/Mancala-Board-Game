#include "Game.h"

Game::Game(const Board& b, Player* south, Player* north): mBoard(b), mSouthPlayer(south), mNorthPlayer(north), currentSide(SOUTH){}
void Game::display() const{
    cout << "   "  << mNorthPlayer->name() << endl << "  ";
    for(int i = 1; i <= mBoard.holes(); i++){
        cout << beans(NORTH, i) << " ";
    }
    cout << endl << mBoard.beans(NORTH, 0);
    for(int i = 0; i <= mBoard.holes()+mBoard.holes(); i++){
        cout << " ";
    }
    cout << mBoard.beans(SOUTH, 0) << endl << "  ";
    for(int i = 1; i <= mBoard.holes(); i++){
        cout << beans(SOUTH, i) << " ";
    }
    cout << endl << "   " << mSouthPlayer->name() << endl;
}
void Game::status(bool& over, bool& hasWinner, Side& winner) const{
    if(mBoard.beansInPlay(NORTH) != 0 && mBoard.beansInPlay(SOUTH) != 0){
        over = false;
    }
    else{
        over = true;
    }
    if(over == true){
        if(mBoard.beansInPlay(SOUTH) + mBoard.beans(SOUTH, POT) > mBoard.beans(NORTH, POT) + mBoard.beansInPlay(NORTH)){
            hasWinner = true;
            winner = SOUTH;
        }
        else if(mBoard.beansInPlay(SOUTH) + mBoard.beans(SOUTH, POT) < mBoard.beans(NORTH, POT) + mBoard.beansInPlay(NORTH)){
            hasWinner = true;
            winner = NORTH;
        }
        else if(mBoard.beansInPlay(SOUTH) + mBoard.beans(SOUTH, POT) == mBoard.beans(NORTH, POT) + mBoard.beansInPlay(NORTH)) hasWinner = false;
    }
}
bool Game::move(){
    if(mBoard.beansInPlay(currentSide) == 0 || mBoard.beansInPlay(opponent(currentSide)) == 0) return false;
    Side es;
    Player* currentPlayer;
    Player* oppCurrentPlayer;
    int eh;
    if(currentSide == SOUTH){ // using pointers to determine which side's turn it is
        currentPlayer = mSouthPlayer;
        oppCurrentPlayer = mNorthPlayer;
    } else {
        currentPlayer = mNorthPlayer;
        oppCurrentPlayer = mSouthPlayer;
    }
    int move1 = currentPlayer->chooseMove(mBoard,currentSide);
    if(!currentPlayer->isInteractive()){ // since we can't print in non-interactive players' chooseMove, print from move
        cout << currentPlayer->name() << " chooses hole " << move1 << endl;
    }
    mBoard.sow(currentSide, move1, es, eh);
    display();
    if(es == currentSide && beans(currentSide, eh) == 1 && beans(opponent(currentSide), eh) != 0 && eh != 0){ // capturing move
        mBoard.moveToPot(opponent(currentSide), eh, currentSide);
        mBoard.moveToPot(currentSide, eh, currentSide);
    }
    if(mBoard.beansInPlay(currentSide) == 0 && mBoard.beansInPlay(opponent(currentSide)) != 0){ // checking again if game is over after all additional moves
        display();
        cout << "Sweeping remaining beans into " << oppCurrentPlayer->name() << "'s pot." << endl;
        for(int i = 1; i <= mBoard.holes(); i++){
            mBoard.moveToPot(SOUTH, i, SOUTH);
            mBoard.moveToPot(NORTH, i, NORTH);
        }
        display();
    }
    else if(mBoard.beansInPlay(currentSide) != 0 && mBoard.beansInPlay(opponent(currentSide)) == 0){
        display();
        cout << "Sweeping remaining beans into " << currentPlayer->name() << "'s pot." << endl;
        for(int i = 1; i <= mBoard.holes(); i++){
            mBoard.moveToPot(SOUTH, i, SOUTH);
            mBoard.moveToPot(NORTH, i, NORTH);
        }
        display();
    }
    else if(eh == POT){
        cout << currentPlayer->name() << " gets another turn!" << endl;
        move(); // recursive call to move() since additional turn
        return true;
    }
    else {
        currentSide = opponent(currentSide);
    }
    return true;
}
void Game::play(){
    display();
    currentSide = SOUTH;
    if(!mSouthPlayer->isInteractive() && !mNorthPlayer->isInteractive()){
        while(mBoard.beansInPlay(NORTH) != 0 && mBoard.beansInPlay(SOUTH) != 0){
            move();
            cout << "Press ENTER to continue" << endl;
            cin.ignore();
        }
    }
    else{
        while(mBoard.beansInPlay(NORTH) != 0 && mBoard.beansInPlay(SOUTH) != 0){
            move();
        }
    }
    bool checkOver;
    bool checkWinner;
    Side winner;
    status(checkOver, checkWinner, winner);
    if(winner == NORTH){
        cout << "The winner is " << mNorthPlayer->name() << "." << endl;
    }
        else if(winner == SOUTH){
            cout << "The winner is  " << mSouthPlayer->name() << "." << endl;
        }
        else{
            cout << "The game resulted in a tie." << endl;
        }
}
int Game::beans(Side s, int hole) const{
    return mBoard.beans(s, hole);
}
