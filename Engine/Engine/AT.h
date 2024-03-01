#pragma once
#include "PossibleMoves.h"
#include <algorithm>
#include <vector>

typedef struct _elemMove {
	mytype Coord[4];
	float asses;
} elemMove;

class Moves {
	std::vector<elemMove> moves;
public:
	mytype len;
	bool ntb;
	Moves();
	void sort(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, int depth, bool turn, int* nodes);
	void fill(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, bool turn);
	bool find(mytype x1, mytype y1, mytype x2, mytype y2);
	float getAsses();
	mytype* getCoord(mytype index);
};

float mmAB(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, int depth, float alpha, float beta, bool turn, int* nodes);

