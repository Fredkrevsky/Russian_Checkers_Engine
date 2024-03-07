#include "Board.h"

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
	else if (mode == 2) {
		x--;
		y++;
		while (CheckCoord(x, y) && (field[x][y] == 0)) {
			if (NTBDamka(field, x--, y++, turn, 2)) {
				return true;
			}
		}
	}
	else if (mode == 3) {
		x++;
		y--;
		while (CheckCoord(x, y) && (field[x][y] == 0)) {
			if (NTBDamka(field, x++, y--, turn, 3)) {
				return true;
			}
		}
	}
	else if (mode == 4) {
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

