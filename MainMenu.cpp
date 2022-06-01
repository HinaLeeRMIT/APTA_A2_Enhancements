#include "MainMenu.h"

MainMenu::MainMenu()
{

}

MainMenu::~MainMenu()
{
    
}

void MainMenu::printMainOptions()
{
    cout << "Menu" << endl;
    cout << "----" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Credits (Show student information)" << endl;
    cout << "4. Quit" << endl;
}

void MainMenu::startMenu()
{
    cout << "Welcome to Scrabble!" << std::endl;
    cout << "-------------------" << std::endl;

    bool gameEnd = false;
    string input;

    while (!gameEnd && !cin.eof())
    {
        printMainOptions();

        //getting input from user
        cout << "> ";
        getline(cin, input);

        //regex cases
        regex case1("1");
        regex case2("2");
        regex case3("3");
        regex case4("4");

        if (regex_match(input, case1)) {
            cout << "\nStarting New Game" << endl;
            cout << "-------------------";
            startGame();
            gameEnd = true;

        } else if (regex_match(input, case2)) {
            loadGame();
            gameEnd = true;

        } else if (regex_match(input, case3)) {
            credits();
            
        } else if (regex_match(input, case4)) {
            cout << "Quitting" << endl;
            gameEnd = true;
            
        } else if (cin.eof()) {
            gameEnd = true;
        } else {
            cout << "Invalid option. Please enter a number between 1 and 4" << endl;
        }
    }
}

void MainMenu::startGame()
{
    // TODO: Create new Game instance. Do not load anything into it.
    // NOTE: Can overload stream operator on Game instance to get access to all the information.
    game = new Game();
    game->startGameLoop();
}

void MainMenu::loadGame()
{
    //TODO load game using overloaded input operator

    cout << "Enter saved game location" << endl;
    cout << "> ";
    string input;
    getline(cin, input);

    //loading in file
    ifstream inFile(input);

    if(inFile.is_open()){
        game = new Game(inFile);
        inFile.close();
        game->startGameFromLoad();
    }
    else{
        cout<<"Unable to load save file " << input << endl;
    }

}

void MainMenu::credits() {
    cout << endl;
    cout << "-------------------" << endl;
    cout << "Name: Hina Lee" << endl
         << "Student ID: 3910654" << endl
         << "Email: s3910654@student.rmit.edu.au" << endl;
    cout << "-------------------" << endl;
    cout << endl;
}