#include "Player.h"

Player::Player() {
    score = 0;
}

Player::Player(std::string name){
    this->name = name;
    hand = new PlayerHand();
    score = 0;
    turnsPassed = 0;
}

Player::~Player(){

}

//setting a players hand
void Player::setPlayerHand(TileBag* bag){
    for(int i = 0; i < 7; i++) {
        hand->addTile(bag->drawTile());
    }
}

PlayerHand* Player::getHand() {
    return hand;
}

void Player::setHand(PlayerHand* hand) {
    this->hand = hand;
}

//Play x tiles
void Player::placeTile(Tile* tileToPlay){
    if (hand->contains(tileToPlay)){
        hand->deleteTile(tileToPlay);
        turnsPassed = 0;
        std::cout << "Playing card: " << tileToPlay->letter << "-" << tileToPlay->value << std::endl;
    }
    else{
        std::cout << "You do not have this tile in your hand!" << std::endl;
    }
}

bool Player::hasTileLetter(char letter) {
    return hand->containsLetter(letter);
}

Tile* Player::placeTileFromLetter(char letter) {
    return hand->getTileFromLetter(letter);
}

Tile* Player::getTileFromLetter(char letter) {
    return hand->getTileFromLetter(letter);
};


void Player::replaceTile(Tile* toReplace, Tile* fromBag) {
    hand->deleteTile(toReplace);
    hand->addTile(fromBag);
    }

//Add Score
void Player::addScore(int addScore){
    score += addScore;
}

int Player::getScore() {
    return score;
}

bool Player::turnsPassedTwice(){
    return turnsPassed >= 2;
}

void Player::passTurn(){
    turnsPassed++;
}

string Player::getName() {
    return name;
}

void Player::setName(string name) {
    this->name = name;
}

ifstream &operator>>(ifstream& inFile,Player &player) {
    //reading name
    string name = "";
    getline(inFile, name);
    player.setName(name);

    //reading score
    int score;
    inFile >> score;
    inFile.ignore();
    player.addScore(score);

    //reading playerhand

    PlayerHand* hand = new PlayerHand();

    inFile >> *hand;
    player.setHand(hand);


    return inFile;
}

std::ofstream &operator<<(std::ofstream& outFile, Player &player) {
    //getting name
    string name = player.getName();
    outFile << name << endl;

    //getting score
    int score = player.getScore();
    outFile << score << endl;

    //getting hand
    PlayerHand *hand = player.getHand();
    outFile << *hand;

    return outFile;
}