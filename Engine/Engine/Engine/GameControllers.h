#pragma once
#include "Engine.h"
#include <vector>

using std::vector;

class GameController final {
public:
    TField field;
    float assess;
    int curr, head;
    bool turn;

    vector<MoveData> gameMoves;
    GameController();
    MOVE_RESULT PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2);
    MOVE_RESULT EngineMove(mytype depth);
    void getPrev();
    void getNext();
    void getCurr();

protected:
    Engine engine;
    MOVE_TYPE type;
    mytype x, y, vec;
    bool locked = false;

    void getData(MoveData& source);
    void setData(MoveData& dest);

};

class AnalysicsController final {
public:
    TField field;
    float assess;
    MOVE_STATUS comment;
    mytype x1, y1, x2, y2;

    vector<MoveData> gameMoves;
    AnalysicsController();
    void evaluate(int index, int depth);
    void setMoves(vector<MoveData>& tgameMoves);
    void getPrev();
    void getNext();
    void getCurr();

private:
    bool turn;

    Engine engine;
    MOVE_TYPE type;
    mytype x, y, vec;
    int curr, head;

    void getData(MoveData& source);
};
