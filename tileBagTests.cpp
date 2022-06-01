#include "TileBag.h"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

int main(void) {
    cout << "----- Tile Bag Tests -----" << endl;
    TileBag* tileBag = new TileBag();
    cout << "building bag" << endl;
    tileBag->buildBag();
    cout << "TileBag built" << endl;

    cout << "Bag contents: " << *tileBag;
    cout << "bag size: " << tileBag->getSize() << endl;

    Tile* t = tileBag->drawTile();

    cout << "Tile drawn, letter: " << t->letter << endl;

    cout << "New Bag Contents: "; 
    cout << *tileBag << endl;
    cout << "New bag size: " << tileBag->getSize() << endl;

    tileBag->addTile(t);
    cout << "Adding tile back to bag, new contents: " << *tileBag;

    cout << "New bag size: " << tileBag->getSize() << endl;

    cout << "Saving bag to file named \"bagTest.out\" contents should match file above" << endl;

    std::ofstream outFile("bagTest.out");

    outFile << *tileBag;
    outFile.close();
}

void testSaveFile() {

}