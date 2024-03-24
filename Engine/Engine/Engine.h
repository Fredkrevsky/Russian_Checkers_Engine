#pragma once
#include "PossibleMoves.h"
#include <algorithm>
#include <vector>

//Время обработки одного узла = 5.61224 * 10^(-6) секунд

//Обновлено: время обработки одного узла: 3,9 * 10^(-7) секунд

typedef struct _MoveData {
    TField field;
    MOVE_TYPE type;
    mytype x, y, vector;
    mytype coord[4];
    float assess;
} MoveData;

enum MOVE_RESULT {
    INVALID_COORD,
    ONE_MORE,
    SUCCESS,
    WIN,
    DRAW,
    LOSE,
};

class Engine {
private:
    mytype find(mytype x1, mytype y1, mytype x2, mytype y2);
    void fill(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, int depth, bool turn);
    void fill(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, bool turn);
    float mmAB(TField& field, mytype x, mytype y, mytype vector, int depth, float alpha, float beta, bool turn);
    float mmAB(TField& field, int depth, float alpha, float beta, bool turns);
public:
    int duration = 0;
    std::vector<MoveData> moves;
    MOVE_RESULT PlayerMove(MoveData* data, bool turn);
    MOVE_RESULT EngineMove(MoveData* data, bool turn, mytype depth);
};



