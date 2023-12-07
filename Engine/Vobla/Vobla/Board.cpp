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
bool CheckCoord(mytype x, mytype y) {
	return (x >= 0) && (y >= 0) && (x < 8) && (y < 8);
}

float AssesDamka[8][8] = {
{1,    0,    1,    0,    1,    0,    1,    0},
{0,    1,    0,    0.93, 0,    0.93, 0,    1},
{1,    0,    1,    0,    0.93, 0,    0.93, 0},
{0,    0.93, 0,    1,    0,    0.93, 0,    1},
{1,    0,    0.93, 0,    1,    0,    0.93, 0},
{0,    0.93, 0,    0.93, 0,    1,    0,    1},
{1,    0,    0.93, 0,    0.93, 0,    1,    0},
{0,    1,    0,    1,    0,    1,    0,    1}
};

float AssesSimple[8][8] = {
{0.95, 0,    0.95, 0,    1,    0,    1,    0},
{0,    0.93, 0,    0.95, 0,    0.95, 0,    0.93},
{0.93, 0,    0.95, 0,    0.95, 0,    0.93, 0},
{0,    0.95, 0,    1,    0,    1,    0,    0.93},
{0.93, 0,    1,    0,    1,    0,    0.95, 0},
{0,    0.93, 0,    0.95, 0,    0.95, 0,    0.93},
{0.93, 0,    0.95, 0,    0.95, 0,    0.93, 0},
{0,    1,    0,    1,    0,    0.95, 0,    0.95}
};

void Board::FillZeros(mytype x1, mytype y1, mytype x2, mytype y2) {
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
bool Board::NTBSimplePiece(mytype x, mytype y, bool turn) {
	mytype temp = 1;
	if (turn) {
		temp = 2;
	}
	if ((x > 1) && (y > 1)) {
		if (((Field[x - 1][y - 1] == temp) || (Field[x - 1][y - 1] == temp + 2)) && (Field[x - 2][y - 2] == 0)) {
			return true;
		}
	}
	if ((x < 6) && (y > 1)) {
		if (((Field[x + 1][y - 1] == temp) || (Field[x + 1][y - 1] == temp + 2)) && (Field[x + 2][y - 2] == 0)) {
			return true;
		}
	}
	if ((x > 1) && (y < 6)) {
		if (((Field[x - 1][y + 1] == temp) || (Field[x - 1][y + 1] == temp + 2)) && (Field[x - 2][y + 2] == 0)) {
			return true;
		}
	}
	if ((x < 6) && (y < 6)) {
		if (((Field[x + 1][y + 1] == temp) || (Field[x + 1][y + 1] == temp + 2)) && (Field[x + 2][y + 2] == 0)) {
			return true;
		}
	}
	return false;
}
float Board::CountPieces(mytype Number)
{
	float result = 0;
	for (mytype i = 0; i < 8; i++)
	{
		for (mytype j = 0; j < 8; j++)
		{
			if (Field[i][j] == Number)
			{
				if ((Number == 1) || (Number == 2)) {
					result += AssesSimple[j][i];
				}
				else {
					result += AssesDamka[j][i];
				}
			}
		}
	}
	return result;
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
Board::Board(Board& other) {
	for (mytype i = 0; i < 8; i++) {
		for (mytype j = 0; j < 8; j++) {
			this->Field[i][j] = other.Field[i][j];
		}
	}
}
bool Board::NTBDamkaOneMore(mytype x, mytype y, bool turn, mytype mode) {
	if (NTBDamka(x, y, turn, mode)) {
		return true;
	}
	if (mode == 1) {
		x++;
		y++;
		while (CheckCoord(x, y) && (Field[x][y] == 0)) {
			if (NTBDamka(x++, y++, turn, mode)) {
				return true;
			}
		}
	}
	if (mode == 2) {
		x--;
		y++;
		while (CheckCoord(x, y) && (Field[x][y] == 0)) {
			if (NTBDamka(x--, y++, turn, mode)) {
				return true;
			}
		}
	}
	if (mode == 3) {
		x++;
		y--;
		while (CheckCoord(x, y) && (Field[x][y] == 0)) {
			if (NTBDamka(x++, y--, turn, mode)) {
				return true;
			}
		}
	}
	if (mode == 4) {
		x--;
		y--;
		while (CheckCoord(x, y) && (Field[x][y] == 0)) {
			if (NTBDamka(x--, y--, turn, mode)) {
				return true;
			}
		}
	}
	return false;
}
bool Board::NTB(bool turn) {
	mytype temp = 2;
	if (turn) {
		temp = 1;
	}
	for (mytype i = 0; i < 8; i++) {
		for (mytype j = 0; j < 8; j++) {
			if (Field[i][j] == temp) {
				if (NTBSimplePiece(i, j, turn)) {
					return true;
				}
			}
			else if (Field[i][j] == temp + 2) {
				if (NTBDamka(i, j, turn, 0)) {
					return true;
				}
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
	FillZeros(x1, y1, x2, y2);
}
float Board::FillAsses() {
	float asses;
	if ((CountPieces(1) + CountPieces(3) < 0.01)) {
		asses = -100;
	}
	else if (CountPieces(2) + CountPieces(4) < 0.01) {
		asses = 100;
	}
	else {
		asses = CountPieces(1) - CountPieces(2) + 5 * (CountPieces(3) - CountPieces(4));
	}
	return asses;
}
mytype Board::NumberOfPieces() {
	mytype result = 0;
	for (mytype i = 0; i < 8; i++) {
		for (mytype j = 0; j < 8; j++) {
			if (Field[i][j] != 0) {
				result++;
			}
		}
	}
	return result;
}