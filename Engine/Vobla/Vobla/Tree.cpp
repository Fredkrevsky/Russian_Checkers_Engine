/*
#include "Tree.h"
#include <algorithm>

mytype Tree::GetMasMax() {
	float max = children[0]->FillAsses();
	float temp1 = 0;
	mytype index = 0;
	for (mytype i = 1; i < childCount; i++) {
		temp1 = children[i]->FillAsses();
		if (temp1 > max) {
			max = temp1;
			index = i;
		}
	}
	return index;
}
mytype Tree::GetMasMin() {
	float min = children[0]->FillAsses();
	float temp1 = 0;
	mytype index = 0;
	for (mytype i = 1; i < childCount; i++) {
		temp1 = children[i]->FillAsses();
		if (temp1 < min) {
			min = temp1;
			index = i;
		}
	}
	return index;
}
void Tree::invert() {
	for (mytype i = 0; i < childCount / 2; i++) {
		Tree* temp = children[i];
		children[i] = children[childCount - i - 1];
		children[childCount - i - 1] = temp;
	}
}
void Tree::invert(mytype WIDTH) {
	for (mytype i = 0; i < WIDTH / 2; i++) {
		Tree* temp = children[i];
		children[i] = children[WIDTH - i - 1];
		children[WIDTH - i - 1] = temp;
	}
}

Tree::Tree(Coord arr, Board board, bool turn) {
	for (mytype i = 0; i < 4; i++) {
		moves[i] = arr[i];
	}
	asses = board.FillAsses();
	childCount = 0;
	children = nullptr;
	this->turn = turn;
	this->board = board;
}
Tree::~Tree() {
	if (children != nullptr) {
		for (mytype i = 0; i < childCount; i++) {
			if (children[i]) {
				delete children[i];
				children[i] = nullptr;
			}
		}
		delete[] children;
		children = nullptr;
	}
}

void Tree::deleteExcept(mytype index) {
	for (mytype i = 0; i < childCount; i++) {
		if (i != index) {
			delete children[i];
			children[i] = nullptr;
		}
	}
}
void Tree::addChild(Tree* child) {
	if (children == nullptr) {
		children = new Tree * [1];
		children[0] = child;
		childCount = 1;
	}
	else {
		Tree** newChildren = new Tree * [childCount + 1];
		for (mytype i = 0; i < childCount; i++) {
			newChildren[i] = children[i];
		}
		newChildren[childCount] = child;
		delete[] children;
		children = newChildren;
		childCount++;
	}
}
int Tree::NumberOfNodes() {
	int result = 0;
	for (mytype i = 0; i < childCount; i++) {
		result += children[i]->NumberOfNodes();
	}
	return result + 1;
}
void Tree::FillMoves(mytype depth) {
	mytype x1, y1, x2, y2;
	PossibleMoves PM(board, turn);
	PM.FillMoves();
	if (PM.ntb) {
		for (mytype i = 0; i < PM.len(); i++) {
			Board oldBoard = board;
			mytype* temp = PM.GetCoord(i);
			x1 = temp[0];
			y1 = temp[1];
			x2 = temp[2];
			y2 = temp[3];
			if (oldBoard.Field[x1][y1] <= 2) {
				oldBoard.Beat(x1, y1, x2, y2);
			}
			else {
				oldBoard.DamkaBeat(x1, y1, x2, y2, 0);
			}
			mytype mode = GetMode(x1, y1, x2, y2, 0);
			Coord arr = { x1, y1, x2, y2 };
			Tree* NewNode = new Tree(arr, oldBoard, turn);
			addChild(NewNode);
			NewNode->FillBeatsForOne(x2, y2, mode, depth);
		}
	}
	else if (depth > 0) {
		for (mytype i = 0; i < PM.len(); i++) {
			Board oldBoard = board;
			mytype* temp = PM.GetCoord(i);
			x1 = temp[0];
			y1 = temp[1];
			x2 = temp[2];
			y2 = temp[3];
			oldBoard.Move(x1, y1, x2, y2);
			Coord arr = { x1, y1, x2, y2 };
			Tree* NewNode = new Tree(arr, oldBoard, !turn);
			addChild(NewNode);
			NewNode->FillMoves(depth - 1);
		}
	}
}
void Tree::FillBeatsForOne(mytype x, mytype y, mytype mode, mytype depth) {
	PossibleMoves PM(board, turn);
	mytype x2, y2;
	if (board.Field[x][y] <= 2) {
		PM.FillBeatsForOne(x, y);
	}
	else {
		PM.FillDamkaBeatsAfterBeat(x, y, mode);
	}
	if (PM.len() == 0) {
		turn = !turn;
		FillMoves(depth - 1);
	}
	else {
		mytype tempmode = mode;
		for (mytype i = 0; i < PM.len(); i++) {
			Board TempBoard = board;
			mytype* temp = PM.GetCoord(i);
			x2 = temp[2];
			y2 = temp[3];
			if (TempBoard.Field[x][y] <= 2) {
				TempBoard.Beat(x, y, x2, y2);
			}
			else {
				TempBoard.DamkaBeat(x, y, x2, y2, mode);
			}
			tempmode = GetMode(x, y, x2, y2, mode);
			Coord arr = { x, y, x2, y2 };
			Tree* NewNode = new Tree(arr, TempBoard, turn);
			addChild(NewNode);
			NewNode->FillBeatsForOne(x2, y2, tempmode, depth); 
		}
	}
}
void Tree::addMoves(mytype amount) {
	if (childCount == 0) {
		FillMoves(amount);
	}
	else {
		for (mytype i = 0; i < childCount; i++) {
			children[i]->addMoves(amount);
		}
	}
}
void Tree::sort() {
	FillAsses();
	std::sort(children, children + childCount, [](const Tree* a, const Tree* b) {
		return a->asses > b->asses;
		});

	if (!turn) {
		invert();
	}
}
void Tree::sort(mytype WIDTH) {
	for (mytype i = 0; i < WIDTH; i++) {
		children[i]->FillAsses();
	}
	std::sort(children, children + WIDTH, [](const Tree* a, const Tree* b) {
		return a->asses > b->asses;
		});

	if (!turn) {
		invert(WIDTH);
	}
}
float Tree::FillAsses() {
	if (childCount == 0) {
		return board.FillAsses();
	}	
	mytype index;
	if (this->turn) {
		index = GetMasMax();
	}
	else {
		index = GetMasMin();
	}
	this->asses = children[index]->asses;
	return this->asses;
}
mytype Tree::exists(mytype x1, mytype y1, mytype x2, mytype y2) {
	for (mytype i = 0; i < childCount; i++) {
		if ((children[i]->moves[0] == x1) && (children[i]->moves[1] == y1) && (children[i]->moves[2] == x2) && (children[i]->moves[3] == y2)) {
			return i;
		}
	}
	return -1;
}
*/