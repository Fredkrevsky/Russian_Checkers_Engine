#include "Engine.h"
#include <chrono>

MOVE_ASSES Engine::getStatus(mytype index, float old) {
	if (moves.len == 1) {
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
	nodes = 0;

	BInit(field);

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
MOVE_RESULT Engine::PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2) {
	if (!isActual) {
		moves.fill(field, type, x, y, vector, turn);
		isActual = true;
	}
	if (moves.find(x1, y1, x2, y2)) {
		x = x2;
		y = y2;
		if (moves.ntb) {
			type = BEAT;
			if (field[x1][y1] >= 3) {
				DamkaBeat(field, x1, y1, x2, y2, vector);
			}
			else {
				Beat(field, x1, y1, x2, y2);
			}
			vector = GetMode(x1, y1, x2, y2, vector);
			moves.fill(field, type, x, y, vector, turn);
			isActual = true;

			if (moves.ntb) {
				return ONE_MORE;
			}
		}
		else {
			Move(field, x1, y1, x2, y2);
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
	nodes = 0;
	while (true) {
		moves.fill(field, type, x, y, vector, turn);
		if (moves.len > 0) {
			result = SUCCESS;
			moves.sort(field, type, x, y, vector, depth - amountOfDamka(field) / 2, turn, &nodes);
			asses = moves.getAsses();
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
				if (field[x1][y1] >= 3) {
					DamkaBeat(field, x1, y1, x2, y2, vector);
				}
				else {
					Beat(field, x1, y1, x2, y2);
				}
				vector = GetMode(x1, y1, x2, y2, vector);
			}
			else {
				Move(field, x1, y1, x2, y2);
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

	return result;
}

