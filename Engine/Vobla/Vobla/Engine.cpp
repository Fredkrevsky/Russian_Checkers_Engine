#include "Engine.h"
#include "math.h"

const mytype WIDTH = 5;

float Engine::GetNumber(mytype depth) {
	return exp((depth - 1) * log(8.31794));
}
Engine::Engine(mytype depth) {
	moves = 0;
	pieces = 24;
	turn = true;
	asses = 0.0;
	Coord arr = { 0 };
	root = new Tree(arr, MainBoard, turn);
	firstroot = root;
	this->depth = depth;
	root->FillMoves(depth);
}
Engine::~Engine() {
	delete firstroot;
}
mytype Engine::PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2) {
	if (moves == 15) {
		return 0;
	}
	mytype index = root->exsists(x1, y1, x2, y2);
	if (index == -1) {
		return -1;
	}
	root->deleteExcept(index);
	root = root->children[index];
	MainBoard = root->MainBoard;
	asses = root->asses;
	if ((root->childCount != 0) && (root->turn == this->turn)) {
		return -1;
	}
	turn = !turn;
	return 1;
}
mytype Engine::EngineMove() {
	if (root->childCount == 0) {
		return -1;
	}
	root->sort();
	mytype temp = WIDTH;
	if (temp > root->childCount) {
		temp = root->childCount;
	}
	for (mytype i = 0; i < 2 * depth; i++) {
		if (root->NumberOfNodes() > GetNumber(depth)) {
			break;
		}
		for (mytype j = 0; j < temp; j++) {
			root->children[j]->addMoves(1);
		}
	}
	root->sort(temp);
	asses = root->asses;
	while ((root->childCount != 0) && (root->turn == this->turn)) {
		root->deleteExcept(0);
		root = root->children[0];
		MainBoard = root->MainBoard;
		root->sort();
	}
	if (MainBoard.NumberOfPieces() != pieces) {
		moves = 0;
		pieces = MainBoard.NumberOfPieces();
	}
	else {
		moves++;
	}
	if (root->childCount == 0) {
		return 1;
	}
	turn = !turn;

	return 0;
}

