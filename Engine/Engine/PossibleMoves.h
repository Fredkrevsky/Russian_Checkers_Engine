#pragma once
#include "Board.h"

typedef mytype Coord[4];

enum MOVE_TYPE {
    MOVE,
    BEAT,
};

class PossibleMoves {
private:
    mytype AllMoves[100][4];
    mytype lenmov;
    bool turn;
    TField field;
    void FillDamkaBeatsDiag(mytype x0, mytype y0, mytype x, mytype y, mytype mode);
    void FillDamkaBeatsForOne(mytype x, mytype y, mytype mode);
    void FillDamkaMoves(mytype x, mytype y);
    void Add(mytype x1, mytype y1, mytype x2, mytype y2);
    void FillBeatsForOne(mytype x, mytype y);
    void FillMoves();
public:
    PossibleMoves(TField& field, bool turn);
    bool ntb;
    mytype len() const;
    mytype* GetCoord(mytype index);
    mytype Fill(MOVE_TYPE type, mytype x, mytype y, mytype vector);
};

