#include "PossibleMoves.h"

void PossibleMoves::FillDamkaBeatsDiag(mytype x0, mytype y0, mytype x, mytype y, mytype mode) {
	mytype temp = 1;
	if (turn) {
		temp = 2;
	}
	if (mode == 1) {
		while (CheckCoord(++x, ++y)) {
			if (field[x][y] != 0) {
				break;
			}
		}
		if (CheckCoord(x + 1, y + 1)) {
			if ((field[x + 1][y + 1] == 0) && ((field[x][y] == temp) || (field[x][y] == temp + 2))) {
				this->Add(x0, y0, ++x, ++y);
				if (!NTBDamkaOneMore(field, x++, y++, turn, 1)) {
					while (CheckCoord(x, y) && (field[x][y] == 0)) {
						this->Add(x0, y0, x++, y++);
					}
				}
			}
		}
	}
	if (mode == 2) {
		while (CheckCoord(--x, ++y)) {
			if (field[x][y] != 0) {
				break;
			}
		}
		if (CheckCoord(x - 1, y + 1)) {
			if ((field[x - 1][y + 1] == 0) && ((field[x][y] == temp) || (field[x][y] == temp + 2))) {
				this->Add(x0, y0, --x, ++y);
				if (!NTBDamkaOneMore(field, x--, y++, turn, 2)) {
					while (CheckCoord(x, y) && (field[x][y] == 0)) {
						this->Add(x0, y0, x--, y++);
					}
				}
			}
		}
	}
	if (mode == 3) {
		while (CheckCoord(++x, --y)) {
			if (field[x][y] != 0) {
				break;
			}
		}
		if (CheckCoord(x + 1, y - 1)) {
			if ((field[x + 1][y - 1] == 0) && ((field[x][y] == temp) || (field[x][y] == temp + 2))) {
				this->Add(x0, y0, ++x, --y);
				if (!NTBDamkaOneMore(field, x++, y--, turn, 3)) {
					while (CheckCoord(x, y) && (field[x][y] == 0)) {
						this->Add(x0, y0, x++, y--);
					}
				}
			}
		}
	}
	if (mode == 4) {
		while (CheckCoord(--x, --y)) {
			if (field[x][y] != 0) {
				break;
			}
		}
		if (CheckCoord(x - 1, y - 1)) {
			if ((field[x - 1][y - 1] == 0) && ((field[x][y] == temp) || (field[x][y] == temp + 2))) {
				this->Add(x0, y0, --x, --y);
				if (!NTBDamkaOneMore(field, x--, y--, turn, 4)) {
					while (CheckCoord(x, y) && (field[x][y] == 0)) {
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
	while (CheckCoord(++x, ++y) && (field[x][y] == 0)) {
		this->Add(x0, y0, x, y);
	}
	x = x0;
	y = y0;
	while (CheckCoord(--x, ++y) && (field[x][y] == 0)) {
		this->Add(x0, y0, x, y);
	}
	x = x0;
	y = y0;
	while (CheckCoord(++x, --y) && (field[x][y] == 0)) {
		this->Add(x0, y0, x, y);
	}
	x = x0;
	y = y0;
	while (CheckCoord(--x, --y) && (field[x][y] == 0)) {
		this->Add(x0, y0, x, y);
	}
}
PossibleMoves::PossibleMoves(TField& tfield, bool tturn) {
	BCopy(field, tfield);
	lenmov = 0;
	turn = tturn;
	ntb = false;
}
void PossibleMoves::Add(mytype x1, mytype y1, mytype x2, mytype y2) {
	AllMoves[lenmov][0] = x1;
	AllMoves[lenmov][1] = y1;
	AllMoves[lenmov][2] = x2;
	AllMoves[lenmov][3] = y2;
	lenmov++;
}
mytype PossibleMoves::len() const {
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
		while (CheckCoord(++x, ++y) && (field[x][y] == 0)) {
			FillDamkaBeatsDiag(x0, y0, x, y, 2);
			FillDamkaBeatsDiag(x0, y0, x, y, 3);
		}
	}
	if (mode == 2) {
		while (CheckCoord(--x, ++y) && (field[x][y] == 0)) {
			FillDamkaBeatsDiag(x0, y0, x, y, 1);
			FillDamkaBeatsDiag(x0, y0, x, y, 4);
		}
	}
	if (mode == 3) {
		while (CheckCoord(++x, --y) && (field[x][y] == 0)) {
			FillDamkaBeatsDiag(x0, y0, x, y, 1);
			FillDamkaBeatsDiag(x0, y0, x, y, 4);
		}
	}
	if (mode == 4) {
		while (CheckCoord(--x, --y) && (field[x][y] == 0)) {
			FillDamkaBeatsDiag(x0, y0, x, y, 2);
			FillDamkaBeatsDiag(x0, y0, x, y, 3);
		}
	}
}
void PossibleMoves::FillBeatsForOne(mytype x, mytype y) {
	if (SBCheck(field, x, y, x + 2, y + 2, turn)) {
		this->Add(x, y, x + 2, y + 2);
	}
	if (SBCheck(field, x, y, x - 2, y + 2, turn)) {
		this->Add(x, y, x - 2, y + 2);
	}
	if (SBCheck(field, x, y, x + 2, y - 2, turn)) {
		this->Add(x, y, x + 2, y - 2);
	}
	if (SBCheck(field, x, y, x - 2, y - 2, turn)) {
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
			if (field[i][j] == temp){
				FillBeatsForOne(i, j);
			}
			else if (field[i][j] == temp + 2) {
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
				if (field[x][y] == temp) {
					if (SMCheck(field, x, y, x + 1, y + dy)) {
						this->Add(x, y, x + 1, y + dy);
					}
					if (SMCheck(field, x, y, x - 1, y + dy)) {
						this->Add(x, y, x - 1, y + dy);
					}
				}
				else if (field[x][y] == temp + 2) {
					FillDamkaMoves(x, y);
				}
			}
		}
	}
}
mytype PossibleMoves::Fill(MOVE_TYPE type, mytype x, mytype y, mytype vector) {
	ntb = false;
	if (type == BEAT) {
		if (field[x][y] >= 3) {
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

