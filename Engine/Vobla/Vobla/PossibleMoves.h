#pragma once
#include "Board.h"

class PossibleMoves {
private:
    mytype** AllMoves;
    mytype lenMov;
    mytype SIZE;
    bool turn;
    Board MainBoard;
    void FillDamkaBeatsDiag(mytype x0, mytype y0, mytype x, mytype y, mytype mode);
    void FillDamkaBeatsForOne(mytype x, mytype y, mytype mode);
    void FillDamkaMoves(mytype x, mytype y);
    bool SimpleMoveCheck(mytype x1, mytype y1, mytype x2, mytype y2, bool turn);
    bool SimpleBeatCheck(mytype x1, mytype y1, mytype x2, mytype y2, bool turn);
public:
    PossibleMoves(const Board MainBoard, bool turn);
    ~PossibleMoves();
    void Renew(const TField Field);
    void Add(mytype x1, mytype y1, mytype x2, mytype y2);
    mytype len();
    mytype GetMoveX1(mytype index);
    mytype GetMoveY1(mytype index);
    mytype GetMoveX2(mytype index);
    mytype GetMoveY2(mytype index);
    void Reset();
    void FillDamkaBeatsAfterBeat(mytype x, mytype y, mytype mode);
    void FillBeatsForOne(mytype x, mytype y, mytype mode);
    void FillMoves();
};

