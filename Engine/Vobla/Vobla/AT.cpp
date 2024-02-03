#include "AT.h"
#include <thread>
#include <vector>

//#define THREAD

float mmAB(Board& board, MOVE_TYPE type, mytype x, mytype y, mytype vector, int depth, float alpha, float beta, bool turn) {

	Moves moves;
	moves.fill(board, type, x, y, vector, turn);

	if (type == BEAT && moves.getLen() == 0) {
		return mmAB(board, MOVE, 0, 0, 0, depth - 1, alpha, beta, !turn);
	}

	if (moves.getLen() == 0) {
		if (turn) {
			return -100;
		}
		return 100;
	}

	if ((depth <= 0) && (!moves.ntb)) {
		return board.getAsses();
	}
	if (turn) {
		float maxEval = -100;
		float eval;
		for (int i = 0; i < moves.getLen(); i++) {
			mytype tempVector = vector;
			Board TempBoard = board;

			mytype x1, y1, x2, y2;
			mytype* temp = moves.getCoord(i);
			x1 = temp[0];
			y1 = temp[1];
			x2 = temp[2];
			y2 = temp[3];
			if (moves.ntb) {
				if (TempBoard.Field[x1][y1] >= 3) {
					TempBoard.DamkaBeat(x1, y1, x2, y2, tempVector);
				}
				else {
					TempBoard.Beat(x1, y1, x2, y2);
				}
				tempVector = GetMode(x1, y1, x2, y2, tempVector);
				eval = mmAB(TempBoard, BEAT, x2, y2, tempVector, depth, alpha, beta, turn);
			}
			else {
				TempBoard.Move(x1, y1, x2, y2);
				eval = mmAB(TempBoard, MOVE, 0, 0, 0, depth - 1, alpha, beta, !turn);
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
		for (int i = 0; i < moves.getLen(); i++) {
			mytype tempVector = vector;
			Board TempBoard = board;

			mytype x1, y1, x2, y2;
			mytype* temp = moves.getCoord(i);
			x1 = temp[0];
			y1 = temp[1];
			x2 = temp[2];
			y2 = temp[3];
			if (moves.ntb) {
				if (TempBoard.Field[x1][y1] >= 3) {
					TempBoard.DamkaBeat(x1, y1, x2, y2, tempVector);
				}
				else {
					TempBoard.Beat(x1, y1, x2, y2);
				}
				tempVector = GetMode(x1, y1, x2, y2, tempVector);
				eval = mmAB(TempBoard, BEAT, x2, y2, tempVector, depth, alpha, beta, turn);
			}
			else {
				TempBoard.Move(x1, y1, x2, y2);
				eval = mmAB(TempBoard, MOVE, 0, 0, 0, depth - 1, alpha, beta, !turn);
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
	moves = nullptr;
	len = 0;
	ntb = false;
}
Moves::~Moves() {
	if (moves != nullptr) {
		delete[] moves;
	}
}
void Moves::fill(Board& board, MOVE_TYPE type, mytype x, mytype y, mytype vector, bool turn) {
	PossibleMoves PM(board, turn);
	PM.Fill(type, x, y, vector);
	ntb = PM.ntb;

	len = PM.len();
	if (moves != nullptr) {
		delete[] moves;
	}
	moves = new Move[len];
	for (int i = 0; i < len; i++) {
		mytype* coord = PM.GetCoord(i);
		for (int j = 0; j < 4; j++) {
			moves[i].Coord[j] = coord[j];
		}
		moves[i].asses = 0;
	}
}
mytype Moves::find(mytype x1, mytype y1, mytype x2, mytype y2) {
	for (int i = 0; i < len; i++) {
		if (moves[i].Coord[0] == x1 && moves[i].Coord[1] == y1 && moves[i].Coord[2] == x2 && moves[i].Coord[3] == y2) {
			return i;
		}
	}
	return -1;
}
mytype* Moves::getCoord(mytype index) {
	return moves[index].Coord;
}
mytype Moves::getLen() {
	return len;
}
float Moves::getAsses() {
	if (len > 0) {
		return moves[0].asses;
	}
	return 0;
}

#ifdef THREAD

void obertka(Board board, MOVE_TYPE type, mytype x, mytype y, mytype vector, int depth, float alpha, float beta, bool turn, float* result, mytype index) {

	float temp = mmAB(board, type, x, y, vector, depth, alpha, beta, turn);
	result[index] = temp;

}

void Moves::sort(Board& board, MOVE_TYPE type, mytype x, mytype y, mytype vector, int depth, bool turn) {

	std::vector<std::thread> threads;
	float result[100] = {};

	for (mytype i = 0; i < len; i++) {
		Board TempBoard = board;
		mytype tempVector = vector;

		mytype x1, y1, x2, y2;
		mytype* temp = getCoord(i);
		x1 = temp[0];
		y1 = temp[1];
		x2 = temp[2];
		y2 = temp[3];
		if (ntb) {
			if (TempBoard.Field[x1][y1] >= 3) {
				TempBoard.DamkaBeat(x1, y1, x2, y2, tempVector);
			}
			else {
				TempBoard.Beat(x1, y1, x2, y2);
			}
			tempVector = GetMode(x1, y1, x2, y2, tempVector);
			
			threads.emplace_back(obertka, TempBoard, BEAT, x2, y2, tempVector, depth, -100, 100, turn, result, i);
		}
		else {
			TempBoard.Move(x1, y1, x2, y2);
			threads.emplace_back(obertka, TempBoard, MOVE, 0, 0, 0, depth - 1, -100, 100, !turn, result, i);
		}
	}

	for (auto& thread : threads) {
		thread.join();
	}

	for (int i = 0; i < len; i++) {
		moves[i].asses = result[i];
	}

	for (int i = 0; i < len; i++) {
		for (int j = len - 1; j > i; j--) {
			if (moves[j].asses < moves[j - 1].asses) {
				Move temp = moves[j];
				moves[j] = moves[j - 1];
				moves[j - 1] = temp;
			}
		}
	}
	if (turn) {
		for (int i = 0; i < len / 2; i++) {
			Move temp = moves[i];
			moves[i] = moves[len - i - 1];
			moves[len - i - 1] = temp;
		}
	}

}
#else
void Moves::sort(Board& board, MOVE_TYPE type, mytype x, mytype y, mytype vector, int depth, bool turn) {
	float result[100];

	for (mytype i = 0; i < len; i++) {
		Board TempBoard = board;
		mytype tempVector = vector;

		mytype x1, y1, x2, y2;
		mytype* temp = getCoord(i);
		x1 = temp[0];
		y1 = temp[1];
		x2 = temp[2];
		y2 = temp[3];
		if (ntb) {
			if (TempBoard.Field[x1][y1] >= 3) {
				TempBoard.DamkaBeat(x1, y1, x2, y2, tempVector);
			}
			else {
				TempBoard.Beat(x1, y1, x2, y2);
			}
			tempVector = GetMode(x1, y1, x2, y2, tempVector);
			moves[i].asses = mmAB(TempBoard, BEAT, x2, y2, tempVector, depth, -100, 100, turn);
			result[i] = moves[i].asses;
		}
		else {
			TempBoard.Move(x1, y1, x2, y2);
			moves[i].asses = mmAB(TempBoard, MOVE, 0, 0, 0, depth - 1, -100, 100, !turn);
			result[i] = moves[i].asses;
		}
	}
	for (int i = 0; i < len; i++) {
		for (int j = len - 1; j > i; j--) {
			if (moves[j].asses < moves[j - 1].asses) {
				Move temp = moves[j];
				moves[j] = moves[j - 1];
				moves[j - 1] = temp;
			}
		}
	}
	if (turn) {
		for (int i = 0; i < len / 2; i++) {
			Move temp = moves[i];
			moves[i] = moves[len - i - 1];
			moves[len - i - 1] = temp;
		}
	}

}
#endif

