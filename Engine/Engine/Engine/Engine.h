#pragma once
#include "PossibleMoves.h"
#include <vector>
#include "ThreadPool.h"

#define THREADS

enum MOVE_STATUS {
    FORCED,
    BEST,
    GOOD,
    INACCURACY,
    BLUNDER,
};

struct MoveData {
    TField field;
    TField oldfield;
    MOVE_TYPE type;
    mytype x, y, vec;
    mytype coord[4];
    float assess;
    bool turn;
    MOVE_STATUS comment;
};

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
#ifdef THREADS
    ThreadPool<MoveData> threadPool;
#endif
    mytype find(mytype x1, mytype y1, mytype x2, mytype y2);
    void fill(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, bool turn, int depth);
    void fill(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, bool turn);
    float mmAB(TField& field, mytype x, mytype y, mytype vector, int depth, float alpha, float beta, bool turn);
    float mmAB(TField& field, int depth, float alpha, float beta, bool turns);
public:
    Engine();
    std::vector<MoveData> moves;
    MOVE_RESULT PlayerMove(MoveData& data);
    MOVE_RESULT EngineMove(MoveData& data, mytype depth);
    void evaluate(MoveData& data, mytype depth);
};



