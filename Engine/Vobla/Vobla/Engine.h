#pragma once
#include "Tree.h"

//Время обработки одного узла = 5.61224 * 10^(-6) секунд

//Время просчета для глубин
//1:   < 1 мс
//2:   < 1 мс
//3:   1 - 5 мс
//4:   5 - 20 мс
//5:   20 - 200 мс
//6:   200 мс - 2 с
//7:   2 - 15 с
//8:   15 с - 2 мин 

class Engine {
private:
    mytype depth;
    bool turn;
    mytype moves;
    mytype pieces;
    Tree* firstroot;
    Tree* root;
    float GetNumber(mytype depth);
public:
    Board MainBoard;
    float asses;
    Engine(mytype depth);
    ~Engine();
    mytype PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2);
    mytype EngineMove();
};

