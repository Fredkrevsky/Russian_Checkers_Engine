#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#define mytype short int
typedef mytype Coord[4];
typedef mytype TField[8][8];


mytype GetMode(mytype x1, mytype y1, mytype x2, mytype y2, mytype mode);
bool CheckCoord(mytype x, mytype y);

class Board {
private:
    void FillZeros(mytype x1, mytype y1, mytype x2, mytype y2);
    void PossibleDamka();
    bool NTBDamka(mytype x, mytype y, bool turn, mytype mode);
    bool NTBSimplePiece(mytype x, mytype y, bool turn);
    float CountPieces(mytype Number);
public:
    TField Field;
    Board();
    Board(const TField Field);
    Board(Board& other);
    bool NTBDamkaOneMore(mytype x, mytype y, bool turn, mytype mode);
    bool NTB(bool turn);
    void Move(mytype x1, mytype y1, mytype x2, mytype y2);
    void Beat(mytype x1, mytype y1, mytype x2, mytype y2);
    void DamkaBeat(mytype x1, mytype y1, mytype x2, mytype y2, mytype mode);
    mytype NumberOfPieces();
    float FillAsses();
};

