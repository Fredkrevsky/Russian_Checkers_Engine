#include "Board.h"

mytype GetMode(mytype x1, mytype y1, mytype x2, mytype y2, mytype mode) {
	mytype result = 0;
	if (mode == 0) {
		if (y1 > y2) {
			if (x1 > x2) {
				result = 4;
			}
			else {
				result = 3;
			}
		}
		else {
			if (x1 > x2) {
				result = 2;
			}
			else {
				result = 1;
			}
		}
	}
	else if ((mode == 1) || (mode == 4)) {
		if ((x1 - y1) > (x2 - y2)) {
			result = 2;
		}
		else if ((x1 - y1) < (x2 - y2)) {
			result = 3;
		}
		else {
			result = mode;
		}
	}
	else if ((mode == 2) || (mode == 3)) {
		if ((x1 + y1) > (x2 + y2)) {
			result = 4;
		}
		else if ((x1 + y1) < (x2 + y2)) {
			result = 1;
		}
		else {
			result = mode;
		}
	}
	return result;
}
bool CheckCoord(const mytype x, const mytype y) {
	return (x >= 0) && (x < 8) && (y >= 0) && (y < 8);
}

void Board::PossibleDamka()
{
	for (mytype i = 0; i < 8; i++) {
		if (Field[i][7] == 1)
		{
			Field[i][7] = 3;
		}
		if (Field[i][0] == 2)
		{
			Field[i][0] = 4;
		}
	}
}

TField InitialBoard = {
{1, 0, 1, 0, 0, 0, 2, 0},
{0, 1, 0, 0, 0, 2, 0, 2},
{1, 0, 1, 0, 0, 0, 2, 0},
{0, 1, 0, 0, 0, 2, 0, 2},
{1, 0, 1, 0, 0, 0, 2, 0},
{0, 1, 0, 0, 0, 2, 0, 2},
{1, 0, 1, 0, 0, 0, 2, 0},
{0, 1, 0, 0, 0, 2, 0, 2}
};

const float AssesDamka[8][8] = {
{1,    0,    0.9,  0,    0.9,  0,    0.9,  0},
{0,    1,    0,    0.8,  0,    0.8,  0,    0.9},
{0.9,  0,    1,    0,    0.8,  0,    0.8,  0},
{0,    0.8,  0,    1,    0,    0.8,  0,    0.9},
{0.9,  0,    0.8,  0,    1,    0,    0.8,  0},
{0,    0.8,  0,    0.8,  0,    1,    0,    0.9},
{0.9,  0,    0.8,  0,    0.8,  0,    1,    0},
{0,    0.9,  0,    0.9,  0,    0.9,  0,    1}
};

const float AssesSimple[8][8] = {
{0.8,  0,    0.9,  0,    0.9,  0,    1.2,  0},
{0,    0.8,  0,    0.9,  0,    1,    0,    1},
{0.8,  0,    1,    0,    1.1,  0,    1.4,  0},
{0,    0.9,  0,    1.1,  0,    1.2,  0,    1},
{1,    0,    1,    0,    1.1,  0,    1.4,  0},
{0,    0.9,  0,    1.1,  0,    1.2,  0,    1},
{1,    0,    0.9,  0,    0.9,  0,    1.4,  0},
{0,    0.8,  0,    0.9,  0,    1,    0,    1}
};

bool Board::NTBDamka(mytype x, mytype y, bool turn, mytype mode) {
	mytype temp = 1;
	if (turn) {
		temp = 2;
	}
	mytype x0 = x;
	mytype y0 = y;

	if (mode != 4) {
		while (CheckCoord(++x0, ++y0)) {
			if (Field[x0][y0] != 0) {
				break;
			}
		}
		if ((x0 < 7) && (y0 < 7)) {
			if (((Field[x0][y0] == temp) || (Field[x0][y0] == temp + 2)) && (Field[x0 + 1][y0 + 1] == 0)) {
				return true;
			}
		}
	}
	if (mode != 3) {
		x0 = x;
		y0 = y;

		while (CheckCoord(--x0, ++y0)) {
			if (Field[x0][y0] != 0) {
				break;
			}
		}
		if ((x0 > 0) && (y0 < 7)) {
			if (((Field[x0][y0] == temp) || (Field[x0][y0] == temp + 2)) && (Field[x0 - 1][y0 + 1] == 0)) {
				return true;
			}
		}
	}
	if (mode != 2) {
		x0 = x;
		y0 = y;

		while (CheckCoord(++x0, --y0)) {
			if (Field[x0][y0] != 0) {
				break;
			}
		}
		if ((x0 < 7) && (y0 > 0)) {
			if (((Field[x0][y0] == temp) || (Field[x0][y0] == temp + 2)) && (Field[x0 + 1][y0 - 1] == 0)) {
				return true;
			}
		}
	}
	if (mode != 1) {
		x0 = x;
		y0 = y;

		while (CheckCoord(--x0, --y0)) {
			if (Field[x0][y0] != 0) {
				break;
			}
		}
		if ((x0 > 0) && (y0 > 0)) {
			if (((Field[x0][y0] == temp) || (Field[x0][y0] == temp + 2)) && (Field[x0 - 1][y0 - 1] == 0)) {
				return true;
			}
		}
	}
	return false;
}
bool Board::NTBDamkaOneMore(mytype x, mytype y, bool turn, mytype mode) {
	if (NTBDamka(x, y, turn, mode)) {
		return true;
	}
	if (mode == 1) {
		x++;
		y++;
		while (CheckCoord(x, y) && (Field[x][y] == 0)) {
			if (NTBDamka(x++, y++, turn, 1)) {
				return true;
			}
		}
	}
	if (mode == 2) {
		x--;
		y++;
		while (CheckCoord(x, y) && (Field[x][y] == 0)) {
			if (NTBDamka(x--, y++, turn, 2)) {
				return true;
			}
		}
	}
	if (mode == 3) {
		x++;
		y--;
		while (CheckCoord(x, y) && (Field[x][y] == 0)) {
			if (NTBDamka(x++, y--, turn, 3)) {
				return true;
			}
		}
	}
	if (mode == 4) {
		x--;
		y--;
		while (CheckCoord(x, y) && (Field[x][y] == 0)) {
			if (NTBDamka(x--, y--, turn, 4)) {
				return true;
			}
		}
	}
	return false;
}

Board::Board() {
	for (mytype i = 0; i < 8; i++) {
		for (mytype j = 0; j < 8; j++) {
			Field[i][j] = InitialBoard[i][j];
		}
	}
};
Board::Board(const TField Field) {
	for (mytype i = 0; i < 8; i++) {
		for (mytype j = 0; j < 8; j++) {
			this->Field[i][j] = Field[i][j];
		}
	}
}
Board::Board(const Board& other) {
	for (mytype i = 0; i < 8; i++) {
		for (mytype j = 0; j < 8; j++) {
			Field[i][j] = other.Field[i][j];
		}
	}
}
bool Board::NTBDamkaDiag(mytype* x0, mytype* y0, bool turn, mytype mode) {
	mytype x = *x0;
	mytype y = *y0;
	mytype temp = 1;
	if (turn) {
		temp = 2;
	}
	if (mode == 1) {
		while (CheckCoord(++x, ++y)) {
			if (Field[x][y] != 0) {
				break;
			}
		}
		if ((x < 7) && (y < 7)) {
			if (((Field[x][y] == temp) || (Field[x][y] == temp + 2)) && (Field[x + 1][y + 1] == 0)) {
				*x0 = x;
				*y0 = y;
				return true;
			}
		}
	}
	else if (mode == 2) {
		while (CheckCoord(--x, ++y)) {
			if (Field[x][y] != 0) {
				break;
			}
		}
		if ((x > 0) && (y < 7)) {
			if (((Field[x][y] == temp) || (Field[x][y] == temp + 2)) && (Field[x - 1][y + 1] == 0)) {
				*x0 = x;
				*y0 = y;
				return true;
			}
		}
	}
	else if (mode == 3) {
		while (CheckCoord(++x, --y)) {
			if (Field[x][y] != 0) {
				break;
			}
		}
		if ((x < 7) && (y > 0)) {
			if (((Field[x][y] == temp) || (Field[x][y] == temp + 2)) && (Field[x + 1][y - 1] == 0)) {
				*x0 = x;
				*y0 = y;
				return true;
			}
		}
	}
	else {
		while (CheckCoord(--x, --y)) {
			if (Field[x][y] != 0) {
				break;
			}
		}
		if ((x > 0) && (y > 0)) {
			if (((Field[x][y] == temp) || (Field[x][y] == temp + 2)) && (Field[x - 1][y - 1] == 0)) {
				*x0 = x;
				*y0 = y;
				return true;
			}
		}
	}
	return false;
}
void Board::Move(mytype x1, mytype y1, mytype x2, mytype y2) {
	Field[x2][y2] = Field[x1][y1];
	Field[x1][y1] = 0;
	PossibleDamka();
}
void Board::Beat(mytype x1, mytype y1, mytype x2, mytype y2) {
	Field[x2][y2] = Field[x1][y1];
	Field[x1][y1] = 0;
	Field[(x1 + x2) / 2][(y1 + y2) / 2] = 0;
	PossibleDamka();
}
bool Board::SimpleMoveCheck(mytype x1, mytype y1, mytype x2, mytype y2) {
	if (CheckCoord(x1, y1) && CheckCoord(x2, y2)) {
		if (Field[x2][y2] == 0) {
			return true;
		}
	}
	return false;
}
bool Board::SimpleBeatCheck(mytype x1, mytype y1, mytype x2, mytype y2, bool turn) {
	mytype temp = 1;
	if (turn) {
		temp = 2;
	}
	if (CheckCoord(x1, y1) && CheckCoord(x2, y2)) {
		mytype x0 = (x1 + x2) / 2;
		mytype y0 = (y1 + y2) / 2;
		if (((Field[x0][y0] == temp) || (Field[x0][y0] == temp + 2)) && (Field[x2][y2] == 0)) {
			return true;
		}
	}
	return false;
}
void Board::DamkaBeat(mytype x1, mytype y1, mytype x2, mytype y2, mytype mode) {
	Field[x2][y2] = Field[x1][y1];
	if (mode == 1) {
		while ((x1 + y1) < (x2 + y2)) {
			Field[x1++][y1++] = 0;
		}
	}
	if (mode == 2) {
		while ((x1 - y1) > (x2 - y2)) {
			Field[x1--][y1++] = 0;
		}
	}
	if (mode == 3) {
		while ((x1 - y1) < (x2 - y2)) {
			Field[x1++][y1--] = 0;
		}
	}
	if (mode == 4) {
		while ((x1 + y1) > (x2 + y2)) {
			Field[x1--][y1--] = 0;
		}
	}
	if (x1 > x2) {
		if (y1 > y2) {
			for (mytype i = x1; i > x2; i--) {
				Field[i][y1 + i - x1] = 0;
			}
		}
		else {
			for (mytype i = x1; i > x2; i--) {
				Field[i][y1 - i + x1] = 0;
			}
		}
	}
	else {
		if (y1 > y2) {
			for (mytype i = x1; i < x2; i++) {
				Field[i][y1 - i + x1] = 0;
			}
		}
		else {
			for (mytype i = x1; i < x2; i++) {
				Field[i][y1 + i - x1] = 0;
			}
		}
	}
}
float Board::getAsses() {
	int temp;
	float asses, res1 = 0, res2 = 0, res3 = 0, res4 = 0;
	for (mytype i = 0; i < 8; i++) {
		for (mytype j = 0; j < 8; j++) {
			temp = Field[i][j];
			if (temp == 1) {
				res1 += AssesSimple[j][i];
			}
			else if (temp == 2) {
				res2 += AssesSimple[7 - j][7 - i];
			}
			else if (temp == 3) {
				res3 += AssesDamka[j][i];
			}
			else if (temp == 4) {
				res4 += AssesDamka[j][i];
			}
		}
	}
	if (res1 + res3 < 0.01) {
		asses = -100;
	}
	else if (res2 + res4 < 0.01) {
		asses = 100;
	}
	else {
		asses = res1 - res2 + 5 * (res3 - res4);
	}
	return asses;
}
mytype Board::amountOfDamka() {
	mytype amount = 0;
	for (mytype i = 0; i < 8; i++) {
		for (mytype j = 0; j < 8; j++) {
			if (Field[i][j] >= 3) {
				amount++;
			}
		}
	}
	return amount;
}

