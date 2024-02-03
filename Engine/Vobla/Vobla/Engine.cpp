#include "Engine.h"
#include <chrono>

const mytype WIDTH = 5;

#define CHEAT

MOVE_ASSES Engine::getStatus(mytype index, float old) {
	if (moves.getLen() == 1) {
		return COMPULSORY;
	}
	if (index == 0) {
		return STRONGEST;
	}
	if (old - asses < 0.3 && turn || old - asses > -0.3 && !turn) {
		return NORMAL;
	}
	if (old - asses < 0.9 && turn || old - asses > -0.9 && !turn) {
		return MISTAKE;
	}
	return BLUNDER;
}
Engine::Engine(mytype dep) {
	turn = true;
	asses = 0.0;
	depth = dep;

	type = MOVE;
	vector = 0;
	isActual = false;

	x = 0;
	y = 0;
	duration = 0;
	accuracy = 1;
	status = COMPULSORY;
	oldasses = 0;
	isSorted = false;
}
Engine::~Engine() {		

}
MOVE_RESULT Engine::PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2) {
	if (!isActual) {
		moves.fill(board, type, x, y, vector, turn);
		isActual = true;
	}
	mytype index = moves.find(x1, y1, x2, y2);
	if (index != -1) {
		x = x2;
		y = y2;
		if (moves.ntb) {
			type = BEAT;
			if (board.Field[x1][y1] >= 3) {
				board.DamkaBeat(x1, y1, x2, y2, vector);
			}
			else {
				board.Beat(x1, y1, x2, y2);
			}
			vector = GetMode(x1, y1, x2, y2, vector);

#ifndef CHEAT
			asses = moves.moves[index].asses;
			status = getStatus(index, oldasses);
			oldasses = asses;
#endif		

			moves.fill(board, type, x, y, vector, turn);
			isActual = true;

			if (moves.ntb) {
				return ONE_MORE;
			}
		}
		else {
			board.Move(x1, y1, x2, y2);
#ifndef CHEAT
			asses = moves.moves[index].asses;
			status = getStatus(index, oldasses);
			oldasses = asses;
#endif
		}

		turn = !turn;
		type = MOVE;
		vector = 0;
		isActual = false;
		return SUCCESS;
	}
	return INVALID_COORD;
}
MOVE_RESULT Engine::EngineMove() {
	MOVE_RESULT result = LOSE;
	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
	while (true) {
		moves.fill(board, type, x, y, vector, turn);
		if (moves.getLen() > 0) {
			result = SUCCESS;
			moves.sort(board, type, x, y, vector, depth - board.amountOfDamka() / 2, turn);
			asses = moves.moves[0].asses;
			mytype* coord = moves.getCoord(0);
			mytype x1, y1, x2, y2;
			x1 = coord[0];
			y1 = coord[1];
			x2 = coord[2];
			y2 = coord[3];
			x = x2;
			y = y2;
			if (moves.ntb) {
				type = BEAT;
				if (board.Field[x1][y1] >= 3) {
					board.DamkaBeat(x1, y1, x2, y2, vector);
				}
				else {
					board.Beat(x1, y1, x2, y2);
				}
				vector = GetMode(x1, y1, x2, y2, vector);
			}
			else {
				board.Move(x1, y1, x2, y2);
				break;
			}
		}
		else {
			break;
		}
	}
	std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();

	std::chrono::milliseconds temp = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	duration = temp.count();

	turn = !turn;
	oldasses = asses;
	type = MOVE;
	vector = 0;

#ifndef CHEAT 
	moves.fill(board, type, x, y, vector, turn);
	moves.sort(board, type, x, y, vector, depth - (board.amountOfDamka() + 1) / 2, turn);
	isSorted = true;
	if (moves.getLen() != 0) {
		asses = moves.moves[0].asses;
	}
	isActual = true;
#endif

	return result;
}

