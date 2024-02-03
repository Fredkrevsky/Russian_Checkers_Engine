#include "PossibleMoves.h"

/*
	
ОШИБКА: 
	FillDamkaBeatsDiag: Бить дамкой получается только на клетку вперед после шашки.	

*/

void PossibleMoves::FillDamkaBeatsDiag(mytype x0, mytype y0, mytype x, mytype y, mytype mode) {
	mytype temp = 1;
	if (turn) {
		temp = 2;
	}
	if (mode == 1) {
		while (CheckCoord(++x, ++y)) {
			if (board.Field[x][y] != 0) {
				break;
			}
		}
		if (CheckCoord(x + 1, y + 1)) {
			if ((board.Field[x + 1][y + 1] == 0) && ((board.Field[x][y] == temp) || (board.Field[x][y] == temp + 2))) {
				this->Add(x0, y0, ++x, ++y);
				if (!board.NTBDamkaOneMore(x++, y++, turn, 1)) {
					while (CheckCoord(x, y) && (board.Field[x][y] == 0)) {
						this->Add(x0, y0, x++, y++);
					}
				}
			}
		}
	}
	if (mode == 2) {
		while (CheckCoord(--x, ++y)) {
			if (board.Field[x][y] != 0) {
				break;
			}
		}
		if (CheckCoord(x - 1, y + 1)) {
			if ((board.Field[x - 1][y + 1] == 0) && ((board.Field[x][y] == temp) || (board.Field[x][y] == temp + 2))) {
				this->Add(x0, y0, --x, ++y);
				if (!board.NTBDamkaOneMore(x--, y++, turn, 2)) {
					while (CheckCoord(x, y) && (board.Field[x][y] == 0)) {
						this->Add(x0, y0, x--, y++);
					}
				}
			}
		}
	}
	if (mode == 3) {
		while (CheckCoord(++x, --y)) {
			if (board.Field[x][y] != 0) {
				break;
			}
		}
		if (CheckCoord(x + 1, y - 1)) {
			if ((board.Field[x + 1][y - 1] == 0) && ((board.Field[x][y] == temp) || (board.Field[x][y] == temp + 2))) {
				this->Add(x0, y0, ++x, --y);
				if (!board.NTBDamkaOneMore(x++, y--, turn, 3)) {
					while (CheckCoord(x, y) && (board.Field[x][y] == 0)) {
						this->Add(x0, y0, x++, y--);
					}
				}
			}
		}
	}
	if (mode == 4) {
		while (CheckCoord(--x, --y)) {
			if (board.Field[x][y] != 0) {
				break;
			}
		}
		if (CheckCoord(x - 1, y - 1)) {
			if ((board.Field[x - 1][y - 1] == 0) && ((board.Field[x][y] == temp) || (board.Field[x][y] == temp + 2))) {
				this->Add(x0, y0, --x, --y);
				if (!board.NTBDamkaOneMore(x--, y--, turn, 4)) {
					while (CheckCoord(x, y) && (board.Field[x][y] == 0)) {
						this->Add(x0, y0, x--, y--);
					}
				}
			}
		}
	}
}
	
void PossibleMoves::FillDamkaMoves(mytype x, mytype y) {
	mytype x0 = x;
	mytype y0 = y;
	while (CheckCoord(++x, ++y) && (board.Field[x][y] == 0)) {
		this->Add(x0, y0, x, y);
	}
	x = x0;
	y = y0;
	while (CheckCoord(--x, ++y) && (board.Field[x][y] == 0)) {
		this->Add(x0, y0, x, y);
	}
	x = x0;
	y = y0;
	while (CheckCoord(++x, --y) && (board.Field[x][y] == 0)) {
		this->Add(x0, y0, x, y);
	}
	x = x0;
	y = y0;
	while (CheckCoord(--x, --y) && (board.Field[x][y] == 0)) {
		this->Add(x0, y0, x, y);
	}
}
PossibleMoves::PossibleMoves(Board& board, bool turn) {
	this->board = board;
	SIZE = 10;
	lenmov = 0;
	AllMoves = new mytype* [SIZE];
	for (mytype i = 0; i < SIZE; i++) {
		AllMoves[i] = new mytype[4];
	}
	this->turn = turn;
	ntb = false;
}
PossibleMoves::~PossibleMoves() {
	for (mytype i = 0; i < SIZE; i++) {
		delete[] AllMoves[i];
	}
	delete[] AllMoves;
}
void PossibleMoves::Add(mytype x1, mytype y1, mytype x2, mytype y2) {
	if (lenmov == SIZE) {
		SIZE = SIZE * 3 / 2 + 1;
		mytype** tempArr = new mytype* [SIZE];
		for (mytype i = 0; i < SIZE; i++) {
			tempArr[i] = new mytype[4];
		}
		for (mytype i = 0; i < lenmov; i++) {
			for (mytype j = 0; j < 4; j++) {
				tempArr[i][j] = AllMoves[i][j];
			}
		}
		for (mytype i = 0; i < lenmov; i++) {
			delete[] AllMoves[i];
		}
		delete[] AllMoves;
		AllMoves = tempArr;
	}
	AllMoves[lenmov][0] = x1;
	AllMoves[lenmov][1] = y1;
	AllMoves[lenmov][2] = x2;
	AllMoves[lenmov][3] = y2;
	lenmov++;
}
mytype PossibleMoves::len() {
	return lenmov;
}
mytype* PossibleMoves::GetCoord(mytype index) {
	return AllMoves[index];
}
void PossibleMoves::FillDamkaBeatsForOne(mytype x, mytype y, mytype mode) {
	mytype x0 = x;
	mytype y0 = y;
	if (mode != 1) {
		FillDamkaBeatsDiag(x0, y0, x, y, 4);
	}
	if (mode != 2) {
		FillDamkaBeatsDiag(x0, y0, x, y, 3);
	}
	if (mode != 3) {
		FillDamkaBeatsDiag(x0, y0, x, y, 2);
	}
	if (mode != 4) {
		FillDamkaBeatsDiag(x0, y0, x, y, 1);
	}
	if (mode == 1) {
		while (CheckCoord(++x, ++y) && (board.Field[x][y] == 0)) {
			FillDamkaBeatsDiag(x0, y0, x, y, 2);
			FillDamkaBeatsDiag(x0, y0, x, y, 3);
		}
	}
	if (mode == 2) {
		while (CheckCoord(--x, ++y) && (board.Field[x][y] == 0)) {
			FillDamkaBeatsDiag(x0, y0, x, y, 1);
			FillDamkaBeatsDiag(x0, y0, x, y, 4);
		}
	}
	if (mode == 3) {
		while (CheckCoord(++x, --y) && (board.Field[x][y] == 0)) {
			FillDamkaBeatsDiag(x0, y0, x, y, 1);
			FillDamkaBeatsDiag(x0, y0, x, y, 4);
		}
	}
	if (mode == 4) {
		while (CheckCoord(--x, --y) && (board.Field[x][y] == 0)) {
			FillDamkaBeatsDiag(x0, y0, x, y, 2);
			FillDamkaBeatsDiag(x0, y0, x, y, 3);
		}
	}
}
void PossibleMoves::FillBeatsForOne(mytype x, mytype y) {
	if (board.SimpleBeatCheck(x, y, x + 2, y + 2, turn)) {
		this->Add(x, y, x + 2, y + 2);
	}
	if (board.SimpleBeatCheck(x, y, x - 2, y + 2, turn)) {
		this->Add(x, y, x - 2, y + 2);
	}
	if (board.SimpleBeatCheck(x, y, x + 2, y - 2, turn)) {
		this->Add(x, y, x + 2, y - 2);
	}
	if (board.SimpleBeatCheck(x, y, x - 2, y - 2, turn)) {
		this->Add(x, y, x - 2, y - 2);
	}
}
void PossibleMoves::FillMoves() {
	mytype temp = 2;
	if (turn) {
		temp = 1;
	}
	for (mytype i = 0; i < 8; i++) {
		for (mytype j = 0; j < 8; j++) {
			if (board.Field[i][j] == temp){
				FillBeatsForOne(i, j);
			}
			else if (board.Field[i][j] == temp + 2) {
				FillDamkaBeatsForOne(i, j, 0);

			}
		}
	}
	if (lenmov != 0) {
		ntb = true;
	}
	else {
		mytype dy = -1;
		if (turn) {
			dy = 1;
		}
		for (mytype x = 0; x < 8; x++) {
			for (mytype y = 0; y < 8; y++) {
				if (board.Field[x][y] == temp) {
					if (board.SimpleMoveCheck(x, y, x + 1, y + dy)) {
						this->Add(x, y, x + 1, y + dy);
					}
					if (board.SimpleMoveCheck(x, y, x - 1, y + dy)) {
						this->Add(x, y, x - 1, y + dy);
					}
				}
				else if (board.Field[x][y] == temp + 2) {
					FillDamkaMoves(x, y);
				}
			}
		}
	}
}
mytype PossibleMoves::Fill(MOVE_TYPE type, mytype x, mytype y, mytype vector) {
	ntb = false;
	if (type == BEAT) {
		if (board.Field[x][y] >= 3) {
			FillDamkaBeatsForOne(x, y, vector);
		}
		else {
			FillBeatsForOne(x, y);
		}

		if (lenmov == 0) {
			return -1;
		}
		else {
			ntb = true;
		}
	}

	if (lenmov == 0) {
		FillMoves();
	}
	return lenmov;
}

