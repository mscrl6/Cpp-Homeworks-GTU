#include "ChessBoard.h"
int main()
{
    int option;
    do
    { // game menu
        cout << "-------Welcome to the chess game-------" << endl
             << "1- New game" << endl
             << "2- Load the saved game" << endl
             << "0- Exit" << endl
             << "->To save the game, write 'save' during play " << endl
             << "->To see the scores, write scores during play " << endl
             << "---------------------------------------" << endl
             << "Enter: ";
        cin >> option;
        // new game
        if (option == 1)
        {
            ChessBoard board;
            board.displayTheBoard();
        }
        // load the saved game
        else if (option == 2)
        {
            ChessBoard board(0);
            if (board.loadTheSavedGame(board))
                board.displayTheBoard();
        }
        // exit
        else if (option == 0)
            exit(-1);
        else
            cout << "Invalid input." << endl;
    } while (option != 0);
}