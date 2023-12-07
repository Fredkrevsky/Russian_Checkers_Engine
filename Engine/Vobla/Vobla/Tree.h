#pragma once
#include "PossibleMoves.h"

class Tree {
private:
    void addChild(Tree* child);
    float FillAsses();
    mytype GetMasMax();
    mytype GetMasMin();
    void invert();
    void invert(mytype WIDTH);
public:
    Board MainBoard;
    bool turn;
    float asses;
    Tree** children;
    mytype childCount;
    Coord moves;
    Tree(Coord arr, Board MainBoard, bool turn);
    ~Tree();
    void FillBeatsForOne(mytype x, mytype y, mytype mode, mytype depth);
    void FillMoves(mytype depth);
    void addMoves(mytype amount);
    void deleteExcept(mytype index);
    int NumberOfNodes();
    void sort();
    void sort(mytype WIDTH);
    mytype exsists(mytype x1, mytype y1, mytype x2, mytype y2);
};

