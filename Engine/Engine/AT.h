#pragma once
#include "PossibleMoves.h"
#include <algorithm>

class elemMove {
public:
	mytype Coord[4];
	float asses;
};

class Moves {
	mytype len;
public:
	elemMove* moves;
	bool ntb;
	Moves();
	~Moves();
	void sort(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, int depth, bool turn, int* nodes);
	void fill(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, bool turn);
	mytype find(mytype x1, mytype y1, mytype x2, mytype y2);
	mytype* getCoord(mytype index);
	mytype getLen();
	float getAsses();
};

float mmAB(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, int depth, float alpha, float beta, bool turn, int* nodes);

