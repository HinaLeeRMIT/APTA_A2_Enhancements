#include "MainMenu.h"

MainMenu::MainMenu()
{

}

MainMenu::~MainMenu()
{
    
}

void MainMenu::printMainOptions()
{
    cout << endl;
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
            //dynamically assigning number of players based on user input
            string numPlayersInput;

            cout << "How many players(2-4)?" << endl;
            cout << "> ";
            getline(cin, numPlayersInput);

            regex validNumPlayers("^[2-4]$");

            //looping until valid number of players if invalid entry
            if (!regex_match(numPlayersInput, validNumPlayers)){
                do {
                    cout << "Invalid option. Please enter a number between 2 and 4!" << endl;
                    cout << "> ";
                    getline(cin, numPlayersInput);
                }while (!regex_match(numPlayersInput, validNumPlayers));
            }

            int intNumPlayers = stoi(numPlayersInput);

            cout << "\nStarting New Game" << endl;
            cout << "-------------------";
            startGame(intNumPlayers);
            gameEnd = true;

        } else if (regex_match(input, case2)) {
            if (loadGame()){
                gameEnd = true;
            }
        } else if (regex_match(input, case3)) {
            credits();
            
        } else if (regex_match(input, case4)) {
            gameEnd = true;
            
        } else if (cin.eof()) {
            gameEnd = true;
        } else {
            cout << "Invalid option. Please enter a number between 1 and 4!" << endl;
        }
    }
    cout << "Quitting" << endl;
}

void MainMenu::startGame(int numPlayers)
{
    // TODO: Create new Game instance. Do not load anything into it.
    // NOTE: Can overload stream operator on Game instance to get access to all the information.
    game = new Game();
    game->startGame(numPlayers);
}

bool MainMenu::loadGame()
{
    //checker to see if game is loaded
    bool gameLoaded = false;

    cout << "Enter saved game location" << endl;
    cout << "> ";
    string input;
    getline(cin, input);

    //appending .txt to file name
    input = input + ".txt";

    //loading in file
    ifstream inFile(input);

    if(inFile.is_open()){
        cout << "Loading game!" << endl;
        cout << endl;
        
        game = new Game(inFile);
        inFile.close();
        game->startGameFromLoad();

        gameLoaded = true;
    }
    else{
        cout<<"Unable to load save file " << input << endl;
    }
    return gameLoaded;
}

void MainMenu::credits() {
    cout << endl;
    cout << "Credits:" << endl;
    cout << "-------------------" << endl;
    cout << "Name: Hina Lee" << endl
         << "Student ID: 3910654" << endl
         << "Email: s3910654@student.rmit.edu.au" << endl;
    cout << "-------------------" << endl;
    cout << endl;
}