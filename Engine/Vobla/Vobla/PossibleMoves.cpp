#include "PossibleMoves.h"
#include "math.h"

	void PossibleMoves::FillDamkaBeatsDiag(mytype x0, mytype y0, mytype x, mytype y, mytype mode) {
		mytype temp = 1;
		if (turn) {
			temp = 2;
		}
		if (mode == 1) {
			while (CheckCoord(++x, ++y)) {
				if (MainBoard.Field[x][y] != 0) {
					break;
				}
			}
			if (CheckCoord(x + 1, y + 1)) {
				if ((MainBoard.Field[x + 1][y + 1] == 0) && ((MainBoard.Field[x][y] == temp) || (MainBoard.Field[x][y] == temp + 2))) {
					this->Add(x0, y0, ++x, ++y);
					Board TempBoard = MainBoard;
					TempBoard.DamkaBeat(x0, y0, x, y, 1);
					if (!TempBoard.NTBDamkaOneMore(x++, y++, turn, 1)) {
						while (CheckCoord(x, y) && (TempBoard.Field[x][y] == 0)) {
							this->Add(x0, y0, x++, y++);
						}
					}
				}
			}
		}
		if (mode == 2) {
			while (CheckCoord(--x, ++y)) {
				if (MainBoard.Field[x][y] != 0) {
					break;
				}
			}
			if (CheckCoord(x - 1, y + 1)) {
				if ((MainBoard.Field[x - 1][y + 1] == 0) && ((MainBoard.Field[x][y] == temp) || (MainBoard.Field[x][y] == temp + 2))) {
					this->Add(x0, y0, --x, ++y);
					Board TempBoard = MainBoard;
					TempBoard.DamkaBeat(x0, y0, x, y, 2);
					if (!TempBoard.NTBDamkaOneMore(x--, y++, turn, 2)) {
						while (CheckCoord(x, y) && (TempBoard.Field[x][y] == 0)) {
							this->Add(x0, y0, x--, y++);
						}
					}
				}
			}
		}
		if (mode == 3) {
			while (CheckCoord(++x, --y)) {
				if (MainBoard.Field[x][y] != 0) {
					break;
				}
			}
			if (CheckCoord(x + 1, y - 1)) {
				if ((MainBoard.Field[x + 1][y - 1] == 0) && ((MainBoard.Field[x][y] == temp) || (MainBoard.Field[x][y] == temp + 2))) {
					this->Add(x0, y0, ++x, --y);
					Board TempBoard = MainBoard;
					TempBoard.DamkaBeat(x0, y0, x, y, 3);
					if (!TempBoard.NTBDamkaOneMore(x++, y--, turn, 3)) {
						while (CheckCoord(x, y) && (TempBoard.Field[x][y] == 0)) {
							this->Add(x0, y0, x++, y--);
						}
					}
				}
			}
		}
		if (mode == 4) {
			while (CheckCoord(--x, --y)) {
				if (MainBoard.Field[x][y] != 0) {
					break;
				}
			}
			if (CheckCoord(x - 1, y - 1)) {
				if ((MainBoard.Field[x - 1][y - 1] == 0) && ((MainBoard.Field[x][y] == temp) || (MainBoard.Field[x][y] == temp + 2))) {
					this->Add(x0, y0, --x, --y);
					Board TempBoard = MainBoard;
					TempBoard.DamkaBeat(x0, y0, x, y, 4);
					if (!TempBoard.NTBDamkaOneMore(x--, y--, turn, 4)) {
						while (CheckCoord(x, y) && (TempBoard.Field[x][y] == 0)) {
							this->Add(x0, y0, x--, y--);
						}
					}
				}
			}
		}
	}
	void PossibleMoves::FillDamkaBeatsForOne(mytype x, mytype y, mytype mode) {
		if (mode != 4) {
			FillDamkaBeatsDiag(x, y, x, y, 1);
		}
		if (mode != 3) {
			FillDamkaBeatsDiag(x, y, x, y, 2);
		}
		if (mode != 2) {
			FillDamkaBeatsDiag(x, y, x, y, 3);
		}
		if (mode != 1) {
			FillDamkaBeatsDiag(x, y, x, y, 4);
		}
	}
	void PossibleMoves::FillDamkaMoves(mytype x, mytype y) {
		mytype x0 = x;
		mytype y0 = y;
		while (CheckCoord(++x, ++y) && (MainBoard.Field[x][y] == 0)) {
			this->Add(x0, y0, x, y);
		}
		x = x0;
		y = y0;
		while (CheckCoord(--x, ++y) && (MainBoard.Field[x][y] == 0)) {
			this->Add(x0, y0, x, y);
		}
		x = x0;
		y = y0;
		while (CheckCoord(++x, --y) && (MainBoard.Field[x][y] == 0)) {
			this->Add(x0, y0, x, y);
		}
		x = x0;
		y = y0;
		while (CheckCoord(--x, --y) && (MainBoard.Field[x][y] == 0)) {
			this->Add(x0, y0, x, y);
		}
	}
	bool PossibleMoves::SimpleMoveCheck(mytype x1, mytype y1, mytype x2, mytype y2, bool turn) {
		mytype temp = 2;
		mytype dy = -1;
		if (turn) {
			temp = 1;
			dy = 1;
		}
		if (CheckCoord(x1, y1) && CheckCoord(x2, y2)) {
			if (MainBoard.Field[x1][y1] == temp) {
				if (((x2 == x1 + 1) || (x2 == x1 - 1)) && (y2 == y1 + dy)) {
					if (MainBoard.Field[x2][y2] == 0) {
						return true;
					}
				}
			}
		}
		return false;
	}
	bool PossibleMoves::SimpleBeatCheck(mytype x1, mytype y1, mytype x2, mytype y2, bool turn) {
		mytype temp = 1;
		if (turn) {
			temp = 2;
		}
		if (CheckCoord(x1, y1) && CheckCoord(x2, y2) && (abs(x1 - x2) == abs(y1 - y2)) && (abs(x1 - x2)) == 2) {
			if (MainBoard.Field[x1][y1] == 3 - temp) {
				mytype x0 = (x1 + x2) / 2;
				mytype y0 = (y1 + y2) / 2;
				if (((MainBoard.Field[x0][y0] == temp) || (MainBoard.Field[x0][y0] == temp + 2)) && (MainBoard.Field[x2][y2] == 0)) {
					return true;
				}
			}
		}
		return false;
	}

	PossibleMoves::PossibleMoves(const Board MainBoard, bool turn) {
		this->MainBoard = MainBoard;
		SIZE = 10;
		lenMov = 0;
		AllMoves = new mytype* [SIZE];
		for (mytype i = 0; i < SIZE; i++) {
			AllMoves[i] = new mytype[4];
		}
		this->turn = turn;
	}
	PossibleMoves::~PossibleMoves() {
		for (mytype i = 0; i < SIZE; i++) {
			delete[] AllMoves[i];
		}
		delete[] AllMoves;
	}
	void PossibleMoves::Renew(const TField Field) {
		for (mytype x = 0; x < 8; x++) {
			for (mytype y = 0; y < 8; y++) {
				this->MainBoard.Field[x][y] = Field[x][y];
			}
		}
	}
	void PossibleMoves::Add(mytype x1, mytype y1, mytype x2, mytype y2) {
		if (lenMov == SIZE) {
			SIZE = SIZE * 3 / 2 + 1;
			mytype** tempArr = new mytype* [SIZE];
			for (mytype i = 0; i < SIZE; i++) {
				tempArr[i] = new mytype[4];
			}
			for (mytype i = 0; i < lenMov; i++) {
				for (mytype j = 0; j < 4; j++) {
					tempArr[i][j] = AllMoves[i][j];
				}
			}
			for (mytype i = 0; i < lenMov; i++) {
				delete[] AllMoves[i];
			}
			delete[] AllMoves;
			AllMoves = tempArr;
		}
		AllMoves[lenMov][0] = x1;
		AllMoves[lenMov][1] = y1;
		AllMoves[lenMov][2] = x2;
		AllMoves[lenMov][3] = y2;
		lenMov++;
	}
	mytype PossibleMoves::len() {
		return lenMov;
	}
	mytype PossibleMoves::GetMoveX1(mytype index) {
		return AllMoves[index][0];
	}
	mytype PossibleMoves::GetMoveY1(mytype index) {
		return AllMoves[index][1];
	}
	mytype PossibleMoves::GetMoveX2(mytype index) {
		return AllMoves[index][2];
	}
	mytype PossibleMoves::GetMoveY2(mytype index) {
		return AllMoves[index][3];
	}
	void PossibleMoves::Reset() {
		lenMov = 0;
	}
	void PossibleMoves::FillDamkaBeatsAfterBeat(mytype x, mytype y, mytype mode) {
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
			while (CheckCoord(++x, ++y) && (MainBoard.Field[x][y] == 0)) {
				FillDamkaBeatsDiag(x0, y0, x, y, 2);
				FillDamkaBeatsDiag(x0, y0, x, y, 3);
			}
		}
		if (mode == 2) {
			while (CheckCoord(--x, ++y) && (MainBoard.Field[x][y] == 0)) {
				FillDamkaBeatsDiag(x0, y0, x, y, 1);
				FillDamkaBeatsDiag(x0, y0, x, y, 4);
			}
		}
		if (mode == 3) {
			while (CheckCoord(++x, --y) && (MainBoard.Field[x][y] == 0)) {
				FillDamkaBeatsDiag(x0, y0, x, y, 1);
				FillDamkaBeatsDiag(x0, y0, x, y, 4);
			}
		}
		if (mode == 4) {
			while (CheckCoord(--x, --y) && (MainBoard.Field[x][y] == 0)) {
				FillDamkaBeatsDiag(x0, y0, x, y, 2);
				FillDamkaBeatsDiag(x0, y0, x, y, 3);
			}
		}
	}
	void PossibleMoves::FillBeatsForOne(mytype x, mytype y, mytype mode) {
		if (MainBoard.Field[x][y] <= 2) {
			if (SimpleBeatCheck(x, y, x + 2, y + 2, turn)) {
				this->Add(x, y, x + 2, y + 2);
			}
			if (SimpleBeatCheck(x, y, x - 2, y + 2, turn)) {
				this->Add(x, y, x - 2, y + 2);
			}
			if (SimpleBeatCheck(x, y, x + 2, y - 2, turn)) {
				this->Add(x, y, x + 2, y - 2);
			}
			if (SimpleBeatCheck(x, y, x - 2, y - 2, turn)) {
				this->Add(x, y, x - 2, y - 2);
			}
		}
		else {
			FillDamkaBeatsForOne(x, y, mode);
		}
	}
	void PossibleMoves::FillMoves() {
		mytype temp = 2;
		if (turn) {
			temp = 1;
		}
		if (MainBoard.NTB(turn)) {
			for (mytype i = 0; i < 8; i++) {
				for (mytype j = 0; j < 8; j++) {
					if ((MainBoard.Field[i][j] == temp) || (MainBoard.Field[i][j] == temp + 2)) {
						FillBeatsForOne(i, j, 0);
					}
				}
			}
		}
		else {
			mytype dy = -1;
			if (turn) {
				dy = 1;
			}
			for (mytype x = 0; x < 8; x++) {
				for (mytype y = 0; y < 8; y++) {
					if (MainBoard.Field[x][y] == temp) {
						if (SimpleMoveCheck(x, y, x + 1, y + dy, turn)) {
							this->Add(x, y, x + 1, y + dy);
						}
						if (SimpleMoveCheck(x, y, x - 1, y + dy, turn)) {
							this->Add(x, y, x - 1, y + dy);
						}
					}
					else if (MainBoard.Field[x][y] == temp + 2) {
						FillDamkaMoves(x, y);
					}
				}
			}
		}
	}

