#include "MainMenu.h"

MainMenu::MainMenu()
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
    std::cout << "Welcome to Scrabble!" << std::endl;
    std::cout << "-------------------" << std::endl;

    bool gameEnd = false;
    string input;
    int menuInput = 0;

    while (!gameEnd && !cin.eof())
    {
        printMainOptions();

        bool validInput = false;

        while (!validInput && !cin.eof())
        {
            cout << "> ";
            std::getline(cin, input);

            try
            {
                menuInput = std::stoi(input);

                validInput = menuInput >= 1 && menuInput <= 4;
                if (!validInput)
                {
                    cout << "Invalid option. Please enter a number between 1 and 4" << endl;
                }
            }
            catch (std::exception &err)
            {

                if (!cin.eof())
                {
                    cout << "Invalid option. Please enter a number between 1 and 4" << endl;
                }
            }
        }

        switch (menuInput)
        {
        case 1:
            cout << "Starting New Game" << endl;
            cout << "-------------------";
                startGame();
                gameEnd = true;
            break;
        case 2:
            loadGame();
            gameEnd = true;
            break;
        case 3:
            credits();
            break;

        case 4:
            gameEnd = true;
            break;

        default:
            break;
        }
    }
    cout << "Quitting" << endl;
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
    std::getline(cin, input);

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
    cout << "Name: Sao Kanneh" << endl 
         << "Student ID: 3788996" << endl 
         << "Email: s3788996@student.rmit.eud.au" << endl 
         << endl;

    cout << "Name: Hina Lee" << endl
         << "Student ID: 3910654" << endl
         << "Email: s3910654@student.rmit.edu.au" << endl
         << endl;
    cout << "Name: Jones Mwanza" << endl
         << "Student ID: 3790504" << endl
         << "Email: s3790504@student.rmit.eud.au" << endl
         << endl;
    cout << "-------------------" << endl;
}