/*
#pragma once
#include "PossibleMoves.h"

typedef mytype Coord[4];

class Tree {
private:
    void addChild(Tree* child);
    float FillAsses();
    mytype GetMasMax();
    mytype GetMasMin();
    void invert();
    void invert(mytype WIDTH);
public:
    Board board;
    bool turn;
    float asses;
    Tree** children;
    mytype childCount;
    Coord moves;
    Tree(Coord arr, Board board, bool turn);
    ~Tree();
    void FillBeatsForOne(mytype x, mytype y, mytype mode, mytype depth);
    void FillMoves(mytype depth);
    void addMoves(mytype amount);
    void deleteExcept(mytype index);
    int NumberOfNodes();
    void sort();
    void sort(mytype WIDTH);
    mytype exists(mytype x1, mytype y1, mytype x2, mytype y2);
};
*/

