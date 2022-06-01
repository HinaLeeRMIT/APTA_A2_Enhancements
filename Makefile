.default: all

all: scrabble

test: linkedListTest tileBagTest

clean:
	rm -rf scrabble linkedListTest tileBagTest *.o *.dSYM

scrabble: utils.o Tile.o Node.o LinkedList.o TileBag.o PlayerHand.o Player.o GameBoard.o Game.o MainMenu.o scrabble.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

linkedListTest: Tile.o Node.o LinkedList.o linkedListTest.o
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

tileBagTest: Tile.o Node.o LinkedList.o Tilebag.o tileBagTests.o 
	g++ -Wall -Werror -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++ -Wall -Werror -std=c++14 -g -O -c $^
