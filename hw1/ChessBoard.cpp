#include "ChessBoard.h"
#include <fstream> // file operations (save and load)
#include <math.h>  // abs function

// no parameter constructor for chessboard
ChessBoard::ChessBoard()
{
    whiteScore = 0.0;
    blackScore = 0.0;
    whichPlayer = 0;
    for (int i = 0; i < 8; ++i)
    {
        vector<Pieces> row;
        for (int j = 0; j < 8; ++j)
            row.push_back(Pieces('.', 'e', 0.0));
        board.push_back(row);
    }

    // initialize the board with white pieces
    board[0][0] = Pieces('R', 'W', 5.0);
    board[0][1] = Pieces('N', 'W', 3.0);
    board[0][2] = Pieces('B', 'W', 3.0);
    board[0][3] = Pieces('Q', 'W', 9.0);
    board[0][4] = Pieces('K', 'W', 0.0);
    board[0][5] = Pieces('B', 'W', 3.0);
    board[0][6] = Pieces('N', 'W', 3.0);
    board[0][7] = Pieces('R', 'W', 5.0);

    // white pawns
    for (int i = 0; i < 8; ++i)
        board[1][i] = Pieces('P', 'W', 1.0);

    // initialize the board with black pieces
    board[7][0] = Pieces('r', 'B', 5.0);
    board[7][1] = Pieces('n', 'B', 3.0);
    board[7][2] = Pieces('b', 'B', 3.0);
    board[7][3] = Pieces('q', 'B', 9.0);
    board[7][4] = Pieces('k', 'B', 0.0);
    board[7][5] = Pieces('b', 'B', 3.0);
    board[7][6] = Pieces('n', 'B', 3.0);
    board[7][7] = Pieces('r', 'B', 5.0);

    // black pawns
    for (int i = 0; i < 8; ++i)
        board[6][i] = Pieces('p', 'B', 1.0);
}
ChessBoard::ChessBoard(int)
{
    blackScore = 0;
    whiteScore = 0;
    whichPlayer = 0;
    for (int i = 0; i < 8; ++i)
    {
        vector<Pieces> row;
        for (int j = 0; j < 8; ++j)
            row.push_back(Pieces('.', 'e', 0.0));
        board.push_back(row);
    }
}
// display the board
void ChessBoard::displayTheBoard()
{

    for (int i = 7; i >= 0; --i)
    {
        cout << i + 1 << " |"; // print the left side

        for (auto &x : board[i])
        {
            if (x.getType() != '.')
                cout << x.getType(); // print the pieces and "." (with using auto keyword and range based for loop  )
            else
                cout << ".";
            cout << " ";
        }
        cout << "\n";
    }

    // print the bottom
    cout << "   ---------------\n";

    cout << "   a b c d e f g h\n";
    move(); // call the move function
}

// make legal moves
void ChessBoard::move()
{
    // print which player to play
    if (whichPlayer % 2 == 0)
        cout << "[White's turn]" << endl;
    else
        cout << "[Black's turn]" << endl;
    string move;

    while (1)
    {
        // check if the move leads to check
        if (isItCheck(board, whichPlayer % 2 == 0 ? 'W' : 'B'))
        {
            // check if the game is over, it it's over ask user to play again.
            if (isGameOver(whichPlayer % 2 == 0 ? 'B' : 'W'))
            {
                char playerColor = whichPlayer % 2 == 0 ? 'B' : 'W';
                cout << "Game over." << playerColor << " player win!" << endl;
                cout << "Do you want to play again?" << endl;
                int option;
                do
                {
                    cout << "1- New game" << endl
                         << "2- Load the saved game" << endl
                         << "0- Exit" << endl
                         << "->To save the game, write 'save' during play " << endl
                         << "->To see the scores, write scores during play " << endl
                         << "---------------------------------------" << endl
                         << "Enter: ";
                    cin >> option;
                    if (option == 1) // play again with new board
                    {
                        ChessBoard newBoard;
                        newBoard.displayTheBoard();
                    }
                    else if (option == 2) // load the saved game
                    {
                        ChessBoard newBoard(0);
                        newBoard.loadTheSavedGame(newBoard);
                        newBoard.displayTheBoard();
                    }
                    else if (option == 0) // exit
                        exit(-1);
                    else
                        cout << "Invalid input." << endl;
                } while (option != 0);
            }

            cerr << "Your king is in check!" << endl;
        }
        // get move input from user
        cout << "Enter your move: ";
        cin >> move;
        if (move == "save") // if input is "save", save the pieces' informations to txt file
        {
            saveTheBoardToFile();
        }
        else if (move == "score") // if input is "score", get the current scores
        {
            getScores();
        }
        // check if input and move are legal and print the error messages
        else if (isInputLegal(move) && isMoveLegal(move, true))
        {
            // if input and move is legal, switch the positions
            char currentColumn = move[0];
            int currentColIndex = currentColumn - 'a';
            char currentRow = move[1];
            int currentRowIndex = currentRow - '1';
            Pieces pieceToMove;
            pieceToMove = board[currentRowIndex][currentColIndex];

            char nextColumn = move[2];
            int nextColIndex = nextColumn - 'a';
            char nextRow = move[3];
            int nextRowIndex = nextRow - '1';
            Pieces target = board[nextRowIndex][nextColIndex];

            // promotion for pawn
            if ((pieceToMove.getType() == 'p' && nextRowIndex == 0) || (pieceToMove.getType() == 'P' && nextRowIndex == 7))
            {
                cout << "You can promote the pawn" << endl
                     << "Please choose: " << endl
                     << "1- Queen" << endl
                     << "2- Bishop " << endl
                     << "3- Rook" << endl
                     << "4- Knight" << endl;
                int option;
                do
                {
                    cin >> option;
                    if (option == 1)
                    {
                        pieceToMove.setType(pieceToMove.getColor() == 'W' ? 'Q' : 'q');
                        pieceToMove.setPoint(9);
                    }
                    else if (option == 2)
                    {
                        pieceToMove.setType(pieceToMove.getColor() == 'W' ? 'B' : 'b');
                        pieceToMove.setPoint(3);
                    }
                    else if (option == 3)
                    {
                        pieceToMove.setType(pieceToMove.getColor() == 'W' ? 'R' : 'r');
                        pieceToMove.setPoint(5);
                    }
                    else if (option == 4)
                    {
                        pieceToMove.setType(pieceToMove.getColor() == 'W' ? 'N' : 'n');
                        pieceToMove.setPoint(3);
                    }
                    else
                        cout << "Invalid input." << endl;
                } while (option > 4 || option < 1);
            }

            // the move is legal, calculate the score
            setScore(target);
            // switch positions
            board[currentRowIndex][currentColIndex].setType('.');
            board[currentRowIndex][currentColIndex].setColor('e');
            board[currentRowIndex][currentColIndex].setPoint(0);

            board[nextRowIndex][nextColIndex].setType(pieceToMove.getType());
            board[nextRowIndex][nextColIndex].setColor(pieceToMove.getColor());
            board[nextRowIndex][nextColIndex].setPoint(pieceToMove.getPoint());
            if (isItCheck(board, whichPlayer % 2 == 0 ? 'W' : 'B'))
            {

                // switch the positions back
                board[currentRowIndex][currentColIndex].setType(pieceToMove.getType());
                board[currentRowIndex][currentColIndex].setColor(pieceToMove.getColor());
                board[currentRowIndex][currentColIndex].setPoint(pieceToMove.getPoint());

                board[nextRowIndex][nextColIndex].setType('.');
                board[nextRowIndex][nextColIndex].setColor('e');
                board[nextRowIndex][nextColIndex].setPoint(0);

                continue;
            }
            whichPlayer += 1; // increment the whichPlayer variable
            cout << endl;
            displayTheBoard();
        }
    }
}

// check if input is legal
const bool ChessBoard::isInputLegal(const string &move) const
{
    // the lenght of the input should be 4 character
    if (move.length() > 4)
    {
        cerr << "The lenght of the input is incorrect. Try again." << endl;
        return false;
    }

    //  move[0] and move[2] can only be a to h
    else if (move[0] < 97 || move[0] > 104 || move[2] < 97 || move[2] > 104)
    {
        cerr << "You can only enter a character a to h. Try again." << endl;
        return false;
    }

    //  move[1] and move[3] can only be 1 to 8
    else if (move[1] < '1' || move[1] > '8' || move[3] < '1' || move[3] > '8')
    {
        cerr << "You can only enter a number 1 to 8. Try again." << endl;
        return false;
    }
    return true;
}

// check if move is legal
const bool ChessBoard::isMoveLegal(const string &move, const bool &printMessage)
{
    // piece that is selected to move
    Pieces pieceToMove;
    char currentColumn = move[0];
    int currentColIndex = currentColumn - 'a';
    char currentRow = move[1];
    int currentRowIndex = currentRow - '1';
    pieceToMove = board[currentRowIndex][currentColIndex];

    // piece to be moved to its current location
    char nextColumn = move[2];
    int nextColIndex = nextColumn - 'a';
    char nextRow = move[3];
    int nextRowIndex = nextRow - '1';
    Pieces targetPiece = board[nextRowIndex][nextColIndex];

    // char variables for colors to make the code easier
    char playerColor = pieceToMove.getColor();
    char opponentColor = targetPiece.getColor();

    // check if the type of the piece that is selected to move is '.'. if so return false
    if (pieceToMove.getType() == '.')
    {
        if (printMessage)
            cerr << "You can't move empty spaces." << endl;
        return false;
    }

    // check if the pieces have the same color. if they are the same, return false
    else if (pieceToMove.getColor() == targetPiece.getColor())
    {
        if (printMessage)
            cerr << "The color of the pieces are both " << pieceToMove.getColor()
                 << " invalid move." << endl;
        return false;
    }

    // check if the player tries to move the opponent's pieces
    else if (whichPlayer % 2 == 0 && pieceToMove.getColor() == 'B')
    {
        if (printMessage)
            cerr << "It's not Black's turn." << endl;
        return false;
    }
    else if (whichPlayer % 2 == 1 && pieceToMove.getColor() == 'W')
    {
        if (printMessage)
            cerr << "It's not White's turn." << endl;
        return false;
    }
    else if (targetPiece.getType() == 'k' && targetPiece.getType() == 'K')
    {
        if (printMessage)
            cerr << "You can't eat the king." << endl;
        return false;
    }
    // switch-case for all the pieces
    switch (pieceToMove.getType())
    {
    // black pawns move to down
    case 'p':
    {
        if (targetPiece.getType() == '.' && currentColIndex == nextColIndex) // move down
        {
            if ((currentRowIndex == 6 && nextRowIndex == 4) || (currentRowIndex - 1 == nextRowIndex)) // move down 2 or 1 index
            {
                if (willKingBeUnderAttack(move, playerColor) == false) // check if after the move king will be under attack, if not return true
                    return true;
                else
                {
                    if (printMessage)
                        cerr << "After the move, king will be underattack. Invalid move!" << endl;
                    return false;
                }
            }
        }
        // if the pawn takes the opponent's piece
        else if (targetPiece.getColor() == opponentColor && abs(currentColIndex - nextColIndex) == 1 && currentRowIndex - 1 == nextRowIndex)
        {
            if (willKingBeUnderAttack(move, playerColor) == false) // check if after the move king will be under attack, if not return true
                return true;
            else
            {
                if (printMessage)
                    cerr << "After the move, king will be underattack. Invalid move!" << endl;
                return false;
            }
        }
        if (printMessage)
            cerr << "Invalid move for black pawn." << endl;
        return false;
    }

    case 'P': // white pawns move to up
    {
        if (targetPiece.getType() == '.' && currentColIndex == nextColIndex) // move up
        {
            if ((currentRowIndex == 1 && nextRowIndex == 3) || (currentRowIndex + 1 == nextRowIndex)) // move up 2 or 1 index
            {
                // check if after the move king will be under attack, if not return true
                if (willKingBeUnderAttack(move, playerColor) == false)
                    return true;
                else
                {
                    if (printMessage)
                        cerr << "After the move, king will be underattack. Invalid move!" << endl;
                    return false;
                }
            }
        }
        // if the pawn takes the opponent's piece
        else if (targetPiece.getColor() == opponentColor && abs(currentColIndex - nextColIndex) == 1 && currentRowIndex + 1 == nextRowIndex) // Çapraz hareket (yeme)
        {
            // check if after the move king will be under attack, if not return true
            if (willKingBeUnderAttack(move, playerColor) == false)
                return true;
            else
            {
                if (printMessage)
                    cerr << "After the move, king will be underattack. Invalid move!" << endl;
                return false;
            }
        }
        if (printMessage)
            cerr << "Invalid move for white pawn." << endl;
        return false;
    }

    case 'r': // both black and white rook
    case 'R':
    {
        // horizontal vertical moves are valid. cross moves are not valid
        if (currentColIndex != nextColIndex && currentRowIndex != nextRowIndex)
        {
            if (printMessage)
                cerr << "Invalid move for rook." << endl;
            return false;
        }

        // checking if there is any pieces on the way to the target
        if (!isPathClear(currentRowIndex, currentColIndex, nextRowIndex, nextColIndex, board))
        {
            if (printMessage)
                cerr << "There is a piece on the way to the target. Invalid move." << endl;
            return false;
        }

        // check if after the move king will be under attack, if not return true
        if (willKingBeUnderAttack(move, playerColor) == false)
            return true;
        else
        {
            if (printMessage)
                cerr << "After the move, king will be underattack. Invalid move!" << endl;
            return false;
        }
    }

    case 'b': // both black and white bishop
    case 'B':
    {
        // only cross moves are valid
        if (abs(currentColIndex - nextColIndex) != abs(currentRowIndex - nextRowIndex))
        {
            if (printMessage)
                cerr << "Invalid move for bishop." << endl;
            return false;
        }

        // checking if there is any pieces on the way to the target
        if (!isPathClear(currentRowIndex, currentColIndex, nextRowIndex, nextColIndex, board))
        {
            if (printMessage)
                cerr << "There is a piece on the way to the target. Invalid move." << endl;
            return false;
        }

        // check if after the move king will be under attack, if not return true
        if (willKingBeUnderAttack(move, playerColor) == false)
            return true;
        else
        {
            if (printMessage)
                cerr << "After the move, king will be underattack. Invalid move!" << endl;
            return false;
        }
    }
    case 'n': // both black and white knight
    case 'N':
    {
        // only L moves are valid
        int rowDifference = abs(currentRowIndex - nextRowIndex);
        int colDifference = abs(currentColIndex - nextColIndex);
        if ((rowDifference == 1 && colDifference == 2) || rowDifference == 2 && colDifference == 1)
        {
            // check if after the move king will be under attack, if not return true
            if (willKingBeUnderAttack(move, pieceToMove.getColor()) == false)
                return true;
            else
            {
                if (printMessage)
                    cerr << "After the move, king will be underattack. Invalid move!" << endl;
                return false;
            }
        }
        else
        {
            if (printMessage)
                cerr << "Invalid move for " << pieceToMove.getColor() << " knight." << endl;
            return false;
        }

        break;
    }
    case 'q': // both black and white queen
    case 'Q':
    {
        // horizontal vertical and cross moves are valid
        if (!(currentColIndex == nextColIndex || currentRowIndex == nextRowIndex || abs(currentColIndex - nextColIndex) == abs(currentRowIndex - nextRowIndex)))
        {
            if (printMessage)
                cerr << "Invalid move for queen." << endl;
            return false;
        }

        // checking if there is any pieces on the way to the target
        if (!isPathClear(currentRowIndex, currentColIndex, nextRowIndex, nextColIndex, board))
        {
            if (printMessage)
                cerr << "There is a piece on the way to the target. Invalid move." << endl;
            return false;
        }

        // check if after the move king will be under attack, if not return true
        if (willKingBeUnderAttack(move, playerColor) == false)
            return true;
        else
        {
            if (printMessage)
                cerr << "After the move, king will be underattack. Invalid move!" << endl;
            return false;
        }
    }
    case 'k': // both white and black king
    case 'K':
    {
        // check if king can move to the target. if the target is under attack, return false
        board[nextRowIndex][nextColIndex].setType('.');
        board[nextRowIndex][nextColIndex].setColor('e');
        board[nextRowIndex][nextColIndex].setPoint(0.0);
        if (isTargetUnderAttack(nextRowIndex, nextColIndex, pieceToMove.getColor()))
        {
            if (printMessage)
                cerr << "Invalid move for king. Target is underattack." << endl;

            board[nextRowIndex][nextColIndex].setType(targetPiece.getType());
            board[nextRowIndex][nextColIndex].setColor(targetPiece.getColor());
            board[nextRowIndex][nextColIndex].setPoint(targetPiece.getPoint());
            return false;
        }

        int rowDifference = abs(currentRowIndex - nextRowIndex);
        int colDifference = abs(currentColIndex - nextColIndex);

        if (rowDifference <= 1 && colDifference <= 1)
            return true;
        else
        {
            if (printMessage)
                cerr << "Invalid move for " << pieceToMove.getColor() << " king." << endl;
            return false;
        }
        break;
    }
    default:
    {
        if (printMessage)
            cerr << "!Illegal Move!" << endl;
        break;
    }
    }

    return true;
}

// check if path (the way to the target) is clear
const bool ChessBoard::isPathClear(const int &startRow, const int &startCol, const int &nextRow, const int &nextCol, const vector<vector<Pieces>> &board) const
{
    int rowIncrement = 0;
    int colIncrement = 0;

    // determine the direction
    if (startRow < nextRow)
        rowIncrement = 1;
    else if (startRow > nextRow)
        rowIncrement = -1;

    if (startCol < nextCol)
        colIncrement = 1;
    else if (startCol > nextCol)
        colIncrement = -1;

    int currentRow = startRow + rowIncrement;
    int currentCol = startCol + colIncrement;

    //  check all the pieces on the path
    while (currentRow != nextRow || currentCol != nextCol)
    {
        // if there is a piece on the way, return false
        if (board[currentRow][currentCol].getType() != '.')
            return false;

        currentRow += rowIncrement;
        currentCol += colIncrement;
    }

    // if the path is clear, return true
    return true;
}

void ChessBoard::getScores() const
{
    cout << "White player: " << getWhiteScore() << endl
         << "Black player: " << getBlackScore() << endl
         << endl;
}
void ChessBoard::setScore(const Pieces &target)
{
    double whiteSafetyScore = safetyOfWhitePieces();
    double blackSafetyScore = safetyOfBlackPieces();

    if (target.getColor() == 'B')
    {
        whiteScore += target.getPoint();
        whiteScore += whiteSafetyScore;
    }
    else if (target.getColor() == 'W')
    {
        blackScore += target.getPoint();
        blackScore += blackSafetyScore;
    }
}

const double ChessBoard::safetyOfWhitePieces() const
{
    double total = 0;
    bool threatened;

    // check every white piece on the board
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (board[i][j].getColor() == 'W')
            {
                threatened = false;
                for (int k = 0; k < 8 && !threatened; ++k)
                {
                    for (int m = 0; m < 8; ++m)
                    {
                        if (board[k][m].getColor() == 'B')
                        {
                            if (isPieceThreateningTarget(k, m, i, j, board))
                            {
                                total -= 0.5;
                                threatened = true; // flag for not to count a piece more than one
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return total;
}

const double ChessBoard::safetyOfBlackPieces() const
{
    double total = 0;
    bool threatened;

    // check every black piece on the board
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (board[i][j].getColor() == 'B')
            {
                threatened = false;
                for (int k = 0; k < 8 && !threatened; ++k)
                {
                    for (int m = 0; m < 8; ++m)
                    {
                        if (board[k][m].getColor() == 'W')
                        {
                            if (isPieceThreateningTarget(k, m, i, j, board))
                            {
                                total -= 0.5;
                                threatened = true; // flag for not to count a piece more than one
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return total;
}

// check will king be under attack after the move
const bool ChessBoard::willKingBeUnderAttack(const string &move, const char &playerColor)
{
    auto temp = board; // a temp board

    // make the move on the temp board
    int currentCol = move[0] - 'a';
    int currentRow = move[1] - '1';
    int nextCol = move[2] - 'a';
    int nextRow = move[3] - '1';
    temp[nextRow][nextCol] = temp[currentRow][currentCol];
    temp[currentRow][currentCol] = Pieces('.', 'e', 0.0);

    // find the location of king that is going to be under attack
    int kingRow, kingCol;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (temp[i][j].getColor() == playerColor && (temp[i][j].getType() == 'K' || temp[i][j].getType() == 'k'))
            {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
    }

    // check every opponent piece whether they are threatening the king or not
    char opponentColor = (playerColor == 'W') ? 'B' : 'W';
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (temp[i][j].getColor() == opponentColor)
            {
                if (isPieceThreateningTarget(i, j, kingRow, kingCol, temp))
                {
                    return true; // king will be under attack
                }
            }
        }
    }

    return false; // king won't be under attack after the move
}

// check if king's target is under attack
const bool ChessBoard::isTargetUnderAttack(const int &targetRow, const int &targetCol, const char &playerColor) const
{
    char opponentColor = (playerColor == 'W') ? 'B' : 'W';
    // the logic is the same with willKingBeUnderAttack
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            Pieces piece = board[row][col];
            if (piece.getColor() == opponentColor)
            {
                if (isPieceThreateningTarget(row, col, targetRow, targetCol, board))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

// check if the opponent's pieces are threatening the target that is selected to move
const bool ChessBoard::isPieceThreateningTarget(const int &pieceRow, const int &pieceCol, const int &targetRow, const int &targetCol, const vector<vector<Pieces>> &tempBoard) const
{
    Pieces piece = tempBoard[pieceRow][pieceCol];
    char pieceType = piece.getType();
    char pieceColor = piece.getColor();

    // check for the pawn
    if ((pieceType == 'P' && pieceColor == 'W' && targetRow == pieceRow + 1 && (targetCol == pieceCol + 1 || targetCol == pieceCol - 1)) ||
        (pieceType == 'p' && pieceColor == 'B' && targetRow == pieceRow - 1 && (targetCol == pieceCol + 1 || targetCol == pieceCol - 1)))
    {
        return true;
    }

    // check for the rook
    if ((pieceType == 'R' || pieceType == 'r') && (pieceRow == targetRow || pieceCol == targetCol))
    {
        if (isPathClear(pieceRow, pieceCol, targetRow, targetCol, tempBoard))
        {
            return true;
        }
    }

    // check for the bishop
    if ((pieceType == 'B' || pieceType == 'b') && abs(pieceRow - targetRow) == abs(pieceCol - targetCol))
    {
        if (isPathClear(pieceRow, pieceCol, targetRow, targetCol, tempBoard))
        {
            return true;
        }
    }

    // check for the knight
    if ((pieceType == 'N' || pieceType == 'n') && ((abs(pieceRow - targetRow) == 2 && abs(pieceCol - targetCol) == 1) || (abs(pieceRow - targetRow) == 1 && abs(pieceCol - targetCol) == 2)))
    {
        return true;
    }

    // check for the queen
    if ((pieceType == 'Q' || pieceType == 'q') && (pieceRow == targetRow || pieceCol == targetCol || abs(pieceRow - targetRow) == abs(pieceCol - targetCol)))
    {
        if (isPathClear(pieceRow, pieceCol, targetRow, targetCol, tempBoard))
        {
            return true;
        }
    }

    // check for the king
    if ((pieceType == 'K' || pieceType == 'k') && abs(pieceRow - targetRow) <= 1 && abs(pieceCol - targetCol) <= 1)
    {
        return true;
    }

    // if target is not under attack, return false
    return false;
}
// check if game is over. if so return true
const bool ChessBoard::isGameOver(const char &playerColor)
{
    // check all the moves for all the pieces that has same color with player
    for (int currentRow = 0; currentRow < 8; ++currentRow)
    {
        for (int currentCol = 0; currentCol < 8; ++currentCol)
        {
            if (board[currentRow][currentCol].getColor() == playerColor)
            {
                for (int nextRow = 0; nextRow < 8; ++nextRow)
                {
                    for (int newCol = 0; newCol < 8; ++newCol)
                    {
                        string move(4, ' ');
                        move[0] = static_cast<char>(currentRow + 'a');
                        move[1] = static_cast<char>(currentCol + '1');
                        move[2] = static_cast<char>(nextRow + 'a');
                        move[3] = static_cast<char>(newCol + '1');

                        // check if the random move is legal, and don't print the error messages
                        if (isMoveLegal(move, false))
                        {
                            // create a temp board to see if king is safe after the move
                            vector<vector<Pieces>> tempBoard = board;
                            applyMoveInTempBoard(tempBoard, move);
                            if (!isItCheck(tempBoard, playerColor))
                            {
                                return false; // if the king is safe after the random move, game can continue
                            }
                        }
                    }
                }
            }
        }
    }
    // return true if game is over
    return true;
}

// for check all the possible moves to save the king, apply the moves in a temp board
void ChessBoard::applyMoveInTempBoard(vector<vector<Pieces>> &tempBoard, const string &move)
{
    int startRow = move[1] - '1';
    int startCol = move[0] - 'a';
    int endRow = move[3] - '1';
    int endCol = move[2] - 'a';

    // switch the positions
    tempBoard[endRow][endCol] = tempBoard[startRow][startCol];
    tempBoard[startRow][startCol] = Pieces('.', 'e', 0.0);
}

const bool ChessBoard::isItCheck(const vector<vector<Pieces>> &tempBoard, const char &playerColor) const
{
    int kingRow, kingCol;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (tempBoard[i][j].getColor() == playerColor && (tempBoard[i][j].getType() == 'K' || tempBoard[i][j].getType() == 'k'))
            {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
    }
    if (isTargetUnderAttack(kingRow, kingCol, playerColor))
    {
        return true;
    }
    return false;
}
void ChessBoard::saveTheBoardToFile() const
{
    ofstream file("savedObject.txt");

    if (!file.is_open())
    {
        cerr << "Error opening the file." << endl;
        return;
    }

    // write to the file each piece's type, color and point
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            file << board[i][j].getType() << " " << board[i][j].getColor() << " " << board[i][j].getPoint() << " ";
        }
    }
    file << whiteScore << " ";
    file << blackScore << " ";
    file << pColor;
    cout << "Game is saved" << endl;
    file.close();
}

bool ChessBoard::loadTheSavedGame(ChessBoard &obj)
{
    ifstream file("savedObject.txt");
    if (!file.is_open())
    {
        cerr << "Error opening the file" << endl;
        return false;
    }

    char t, c;
    double p;
    int ws, bs;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            file >> t;
            obj.board[i][j].setType(t);
            file >> c;
            obj.board[i][j].setColor(c);
            file >> p;
            obj.board[i][j].setPoint(p);
        }
    }
    // read from the file scores and color
    file >> ws >> bs;
    obj.setWhiteScore(ws);
    obj.setBlackScore(bs);
    char currentPlayer;
    file >> currentPlayer;
    obj.whichPlayer = (currentPlayer == 'W' ? 1 : 2);
    file.close();
    return true;
}
