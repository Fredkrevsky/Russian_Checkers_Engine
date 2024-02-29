#include "Board.h"

bool NTBDamka(TField& field, mytype x, mytype y, bool turn, mytype mode) {
	mytype temp = 1;
	if (turn) {
		temp = 2;
	}
	mytype x0 = x;
	mytype y0 = y;

	if (mode != 4) {
		while (CheckCoord(++x0, ++y0)) {
			if (field[x0][y0] != 0) {
				break;
			}
		}
		if ((x0 < 7) && (y0 < 7)) {
			if (((field[x0][y0] == temp) || (field[x0][y0] == temp + 2)) && (field[x0 + 1][y0 + 1] == 0)) {
				return true;
			}
		}
	}
	if (mode != 3) {
		x0 = x;
		y0 = y;

		while (CheckCoord(--x0, ++y0)) {
			if (field[x0][y0] != 0) {
				break;
			}
		}
		if ((x0 > 0) && (y0 < 7)) {
			if (((field[x0][y0] == temp) || (field[x0][y0] == temp + 2)) && (field[x0 - 1][y0 + 1] == 0)) {
				return true;
			}
		}
	}
	if (mode != 2) {
		x0 = x;
		y0 = y;

		while (CheckCoord(++x0, --y0)) {
			if (field[x0][y0] != 0) {
				break;
			}
		}
		if ((x0 < 7) && (y0 > 0)) {
			if (((field[x0][y0] == temp) || (field[x0][y0] == temp + 2)) && (field[x0 + 1][y0 - 1] == 0)) {
				return true;
			}
		}
	}
	if (mode != 1) {
		x0 = x;
		y0 = y;

		while (CheckCoord(--x0, --y0)) {
			if (field[x0][y0] != 0) {
				break;
			}
		}
		if ((x0 > 0) && (y0 > 0)) {
			if (((field[x0][y0] == temp) || (field[x0][y0] == temp + 2)) && (field[x0 - 1][y0 - 1] == 0)) {
				return true;
			}
		}
	}
	return false;
}
bool NTBDamkaOneMore(TField& field, mytype x, mytype y, bool turn, mytype mode) {
	if (NTBDamka(field, x, y, turn, mode)) {
		return true;
	}
	if (mode == 1) {
		x++;
		y++;
		while (CheckCoord(x, y) && (field[x][y] == 0)) {
			if (NTBDamka(field, x++, y++, turn, 1)) {
				return true;
			}
		}
	}
	if (mode == 2) {
		x--;
		y++;
		while (CheckCoord(x, y) && (field[x][y] == 0)) {
			if (NTBDamka(field, x--, y++, turn, 2)) {
				return true;
			}
		}
	}
	if (mode == 3) {
		x++;
		y--;
		while (CheckCoord(x, y) && (field[x][y] == 0)) {
			if (NTBDamka(field, x++, y--, turn, 3)) {
				return true;
			}
		}
	}
	if (mode == 4) {
		x--;
		y--;
		while (CheckCoord(x, y) && (field[x][y] == 0)) {
			if (NTBDamka(field, x--, y--, turn, 4)) {
				return true;
			}
		}
	}
	return false;
}
bool NTBDamkaDiag(TField& field, mytype* x0, mytype* y0, bool turn, mytype mode) {
	mytype x = *x0;
	mytype y = *y0;
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
		if ((x < 7) && (y < 7)) {
			if (((field[x][y] == temp) || (field[x][y] == temp + 2)) && (field[x + 1][y + 1] == 0)) {
				*x0 = x;
				*y0 = y;
				return true;
			}
		}
	}
	else if (mode == 2) {
		while (CheckCoord(--x, ++y)) {
			if (field[x][y] != 0) {
				break;
			}
		}
		if ((x > 0) && (y < 7)) {
			if (((field[x][y] == temp) || (field[x][y] == temp + 2)) && (field[x - 1][y + 1] == 0)) {
				*x0 = x;
				*y0 = y;
				return true;
			}
		}
	}
	else if (mode == 3) {
		while (CheckCoord(++x, --y)) {
			if (field[x][y] != 0) {
				break;
			}
		}
		if ((x < 7) && (y > 0)) {
			if (((field[x][y] == temp) || (field[x][y] == temp + 2)) && (field[x + 1][y - 1] == 0)) {
				*x0 = x;
				*y0 = y;
				return true;
			}
		}
	}
	else {
		while (CheckCoord(--x, --y)) {
			if (field[x][y] != 0) {
				break;
			}
		}
		if ((x > 0) && (y > 0)) {
			if (((field[x][y] == temp) || (field[x][y] == temp + 2)) && (field[x - 1][y - 1] == 0)) {
				*x0 = x;
				*y0 = y;
				return true;
			}
		}
	}
	return false;
}
void DamkaBeat(TField& field, mytype x1, mytype y1, mytype x2, mytype y2, mytype mode) {
	field[x2][y2] = field[x1][y1];
	if (mode == 1) {
		while ((x1 + y1) < (x2 + y2)) {
			field[x1++][y1++] = 0;
		}
	}
	if (mode == 2) {
		while ((x1 - y1) > (x2 - y2)) {
			field[x1--][y1++] = 0;
		}
	}
	if (mode == 3) {
		while ((x1 - y1) < (x2 - y2)) {
			field[x1++][y1--] = 0;
		}
	}
	if (mode == 4) {
		while ((x1 + y1) > (x2 + y2)) {
			field[x1--][y1--] = 0;
		}
	}
	if (x1 > x2) {
		if (y1 > y2) {
			for (mytype i = x1; i > x2; i--) {
				field[i][y1 + i - x1] = 0;
			}
		}
		else {
			for (mytype i = x1; i > x2; i--) {
				field[i][y1 - i + x1] = 0;
			}
		}
	}
	else {
		if (y1 > y2) {
			for (mytype i = x1; i < x2; i++) {
				field[i][y1 - i + x1] = 0;
			}
		}
		else {
			for (mytype i = x1; i < x2; i++) {
				field[i][y1 + i - x1] = 0;
			}
		}
	}
}

mytype amountOfDamka(TField& field) {
	mytype amount = 0;
	for (mytype i = 0; i < 8; i++) {
		for (mytype j = 0; j < 8; j++) {
			if (field[i][j] >= 3) {
				amount++;
			}
		}
	}
	return amount;
}

