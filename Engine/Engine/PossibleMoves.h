#pragma once
#include "Board.h"

typedef mytype Coord[4];

enum MOVE_TYPE {
    MOVE,
    BEAT,
};

extern "C" void asmAdd(mytype[100][4], mytype*, mytype, mytype, mytype, mytype);

class PossibleMoves {
private:
    bool turn;
    TField field;
    void FillDamkaBeatsDiag(mytype x0, mytype y0, mytype x, mytype y, mytype mode);
    void FillDamkaBeatsForOne(mytype x, mytype y, mytype mode);
    void FillDamkaMoves(mytype x, mytype y);
    void FillBeatsForOne(mytype x, mytype y);
    void FillMoves();
public:
    mytype AllMoves[100][4];
    mytype len;
    PossibleMoves(TField& field, bool turn);
    bool ntb;
    void fill(MOVE_TYPE type, mytype x, mytype y, mytype vector);
};

