#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include "Pieces.h"

class ChessBoard
{
public:
    ChessBoard();
    ChessBoard(int); // for loading from file

    // display board
    void displayTheBoard();

    // take move input and make move
    void move();

    // check if move is legal
    const bool isMoveLegal(const string &move, const bool &printMessage);
    // check if input is legal
    const bool isInputLegal(const string &move) const;
    // check if the path is clear (queen, bishop, rook)
    const bool isPathClear(const int &startRow, const int &startCol, const int &endRow, const int &endCol, const vector<vector<Pieces>> &board) const;

    // check if will king be under attack after the move
    const bool willKingBeUnderAttack(const string &move, const char& playerColor);
    // check if will king's target is under attack
    const bool isTargetUnderAttack(const int& targetRow, const int&  targetCol, const char&  playerColor) const;
    // check if other player's any piece threats the target
    const bool isPieceThreateningTarget(const int& pieceRow, const int& pieceCol, const int& targetRow, const int& targetCol, const vector<vector<Pieces>> &tempBoard) const;

    // check if the move leads to check
    const bool isItCheck(const vector<vector<Pieces>> &tempBoard, const char &playerColor) const;
    // check whether game is over or not
    const bool isGameOver(const char &playerColor);
    // to apply some moves in temp board
    void applyMoveInTempBoard(vector<vector<Pieces>> &tempBoard, const string &move);

    // functions for file operations
    void saveTheBoardToFile() const;
    bool loadTheSavedGame(ChessBoard &obj);

    // functions for calculate and set the scores
    void getScores() const;
    void setWhiteScore(const double &p) { whiteScore = p; }
    void setBlackScore(const double &p) { blackScore = p; }
    const double getWhiteScore() const { return whiteScore; }
    const double getBlackScore() const { return blackScore; }
    void setScore(const Pieces &target);
    const double safetyOfWhitePieces() const;
    const double safetyOfBlackPieces() const;


private:
    vector<vector<Pieces>> board;
    double blackScore;
    double whiteScore;
    int whichPlayer;
    char pColor = whichPlayer % 2 == 0 ? 'W' : 'B';
};

#endif // CHESSBOARD_H