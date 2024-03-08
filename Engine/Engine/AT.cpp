#include "AT.h"
#include <vector>

float mmAB(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, int depth, float alpha, float beta, bool turn, int* nodes) {

	(*nodes)++;

	TAM AllMoves;
	mytype len = 0;
	bool ntb = PMFill(field, type, AllMoves, &len, turn, x, y, vector);

	if (type && len == 0) {
		return mmAB(field, MOVE, 0, 0, 0, depth - 1, alpha, beta, !turn, nodes);
	}

	if (len == 0) {
		if (turn) {
			return -100;
		}
		return 100;
	}

	if ((depth <= 0) && (!ntb)) {
		return (float)getAssess(field)/10;
	}
	if (turn) {
		float maxEval = -100;
		float eval;
		for (int i = 0; i < len; i++) {
			mytype tempVector = vector;
			TField TempBoard;
			BCopy(TempBoard, field);

			mytype x1, y1, x2, y2;
			mytype* temp = AllMoves[i];
			x1 = temp[0];
			y1 = temp[1];
			x2 = temp[2];
			y2 = temp[3];
			if (ntb) {
				if (TempBoard[x1][y1] >= 3) {
					DamkaBeat(TempBoard, x1, y1, x2, y2, tempVector);
				}
				else {
					Beat(TempBoard, x1, y1, x2, y2);
				}
				tempVector = GetMode(x1, y1, x2, y2, tempVector);
				eval = mmAB(TempBoard, BEAT, x2, y2, tempVector, depth, alpha, beta, turn, nodes);
			}
			else {
				Move(TempBoard, x1, y1, x2, y2);
				eval = mmAB(TempBoard, MOVE, 0, 0, 0, depth - 1, alpha, beta, !turn, nodes);
			}
			maxEval = std::max(maxEval, eval);
			alpha = std::max(alpha, eval);
			if (beta <= alpha) {
				break;
			}
		}
		return maxEval;
	}
	else {
		float minEval = 100;
		float eval;
		for (int i = 0; i < len; i++) {
			mytype tempVector = vector;
			TField TempBoard;
			BCopy(TempBoard, field);

			mytype x1, y1, x2, y2;
			mytype* temp = AllMoves[i];
			x1 = temp[0];
			y1 = temp[1];
			x2 = temp[2];
			y2 = temp[3];
			if (ntb) {
				if (TempBoard[x1][y1] >= 3) {
					DamkaBeat(TempBoard, x1, y1, x2, y2, tempVector);
				}
				else {
					Beat(TempBoard, x1, y1, x2, y2);
				}
				tempVector = GetMode(x1, y1, x2, y2, tempVector);
				eval = mmAB(TempBoard, BEAT, x2, y2, tempVector, depth, alpha, beta, turn, nodes);
			}
			else {
				Move(TempBoard, x1, y1, x2, y2);
				eval = mmAB(TempBoard, MOVE, 0, 0, 0, depth - 1, alpha, beta, !turn, nodes);
			}

			minEval = std::min(minEval, eval);
			beta = std::min(beta, eval);
			if (beta <= alpha) {
				break;
			}
		}
		return minEval;
	}
}

Moves::Moves() {
	len = 0;
	ntb = false;
}
void Moves::fill(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, bool turn) {
	TAM AllMoves;
	len = 0;
	ntb = PMFill(field, type, AllMoves, &len, turn, x, y, vector);
	moves.clear();

	elemMove temp;
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < 4; j++) {
			temp.Coord[j] = AllMoves[i][j];
		}
		temp.asses = 0;
		moves.push_back(temp);
	}
}
bool Moves::find(mytype x1, mytype y1, mytype x2, mytype y2) {
	for (auto elem: moves) {
		if (elem.Coord[0] == x1 && elem.Coord[1] == y1 && elem.Coord[2] == x2 && elem.Coord[3] == y2) {
			return true;
		}
	}
	return false;
}
float Moves::getAsses() {
	if (len > 0) {
		return moves[0].asses;
	}
	return 0;
}
mytype* Moves::getCoord(mytype index) {
	return moves[index].Coord;
}
void Moves::sort(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, int depth, bool turn, int* nodes) {

	for (mytype i = 0; i < len; i++) {
		TField TempBoard;
		BCopy(TempBoard, field);
		mytype tempVector = vector;

		mytype x1, y1, x2, y2;
		elemMove temp = moves[i];
		x1 = temp.Coord[0];
		y1 = temp.Coord[1];
		x2 = temp.Coord[2];
		y2 = temp.Coord[3];
		if (ntb) {
			if (TempBoard[x1][y1] >= 3) {
				DamkaBeat(TempBoard, x1, y1, x2, y2, tempVector);
			}
			else {
				Beat(TempBoard, x1, y1, x2, y2);
			}
			tempVector = GetMode(x1, y1, x2, y2, tempVector);
			moves[i].asses = mmAB(TempBoard, BEAT, x2, y2, tempVector, depth, -100, 100, turn, nodes);
		}
		else {
			Move(TempBoard, x1, y1, x2, y2);
			moves[i].asses = mmAB(TempBoard, MOVE, 0, 0, 0, depth - 1, -100, 100, !turn, nodes);
		}
	}
	for (int i = 0; i < len; i++) {
		for (int j = len - 1; j > i; j--) {
			if (moves[j].asses < moves[j - 1].asses) {
				elemMove temp = moves[j];
				moves[j] = moves[j - 1];
				moves[j - 1] = temp;
			}
		}
	}
	if (turn) {
		for (int i = 0; i < len / 2; i++) {
			elemMove temp = moves[i];
			moves[i] = moves[len - i - 1];
			moves[len - i - 1] = temp;
		}
	}
}

