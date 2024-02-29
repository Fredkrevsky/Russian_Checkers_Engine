#pragma once

#include "AT.h"

//Время обработки одного узла = 5.61224 * 10^(-6) секунд

//Обновлено: время обработки одного узла: 7,1 * 10^(-7) секунд

//Время просчета для глубин       AT
//1:   < 1 мс                     < 1 мс     
//2:   < 1 мс                     < 1 мс
//3:   1 - 5 мс                   < 1 мс
//4:   5 - 20 мс                  2 - 15 мс 
//5:   20 - 200 мс                5 - 30 мс
//6:   200 мс - 2 с               30 - 100 мс
//7:   2 - 15 с                   100 - 200 мс
//8:   15 с - 2 мин 
//9:   2+ мин
//10:  12+ мин


enum MOVE_RESULT {
    INVALID_COORD,
    ONE_MORE,
    SUCCESS,
    WIN,
    DRAW,
    LOSE,
};

enum MOVE_ASSES {
    COMPULSORY,
    BRILLIANT,
    STRONGEST,
    NORMAL,
    MISTAKE,
    BLUNDER
};

class Engine {
private:
    mytype depth;
    bool turn;
    bool isActual;
    MOVE_TYPE type;
    mytype vector;
    mytype x, y;
    MOVE_ASSES getStatus(mytype index, float old);
    float oldasses;
    bool isSorted;
public:
    int nodes;
    Moves moves;
    int duration;
    TField field;
    float asses;
    Engine(mytype depth);
    ~Engine();
    MOVE_ASSES status;
    float accuracy;
    MOVE_RESULT PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2);
    MOVE_RESULT EngineMove();
};

