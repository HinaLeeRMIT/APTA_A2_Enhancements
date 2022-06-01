#include "LinkedList.h"

#include <iostream>
#include <string>

int main(void) {
    LinkedList *list = new LinkedList();
    std::cout << "Testing List" << std::endl;
    list->addFront(new Tile('A', 1));
    std::cout << "Add front, head =" << list->get(0)->letter << std::endl;

    list->addFront(new Tile('B', 1));
    std::cout << "Add front again, new head=" << list->get(0)->letter << std::endl;

    std::cout << "length: expect=2, actual=" << list->getLength() << std::endl;

    list->addBack(new Tile('C', 1));

    std::cout << "add tail, expected=C, actual=" << list->getTailTile()->letter << std::endl;

    Tile *headTile = list->shift();

    std::cout << "list shift; headTile=" << headTile->letter
              << " new list head: expected=A, actual=" << list->getHeadTile()->letter << std::endl;

    list->addBack(new Tile('D', 1));

    std::cout << "addBack, expected_tail=D, actual=" << list->getTailTile()->letter << std::endl;

    Tile* tailTile = list->pop();

    std::cout << "list pop: tailTile=" << tailTile->letter << " new list tail:"  << list->getTailTile()->letter << std::endl;

    list->addFront(new Tile('B', 1));

    std::cout << "add front. expectedHead=B, actual=" << list->getHeadTile()->letter << std::endl;


    std::string listStr = "";

    std::cout << "current list: " << listStr << std::endl;

    std::cout << "removing index 1, A should not be in list anymore." << std::endl;

    list->remove(1);

    listStr = "";

    for (int i = 0; i < list->getLength(); i++) {
        listStr.push_back(list->get(i)->letter);
        listStr += ", ";
    }

    std::cout << "current list: " << listStr << std::endl;

    delete list;
}