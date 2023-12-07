#pragma once
#include "Tree.h"

//����� ��������� ������ ���� = 5.61224 * 10^(-6) ������

//����� �������� ��� ������
//1:   < 1 ��
//2:   < 1 ��
//3:   1 - 5 ��
//4:   5 - 20 ��
//5:   20 - 200 ��
//6:   200 �� - 2 �
//7:   2 - 15 �
//8:   15 � - 2 ��� 

class Engine {
private:
    mytype depth;
    bool turn;
    mytype moves;
    mytype pieces;
    Tree* firstroot;
    Tree* root;
    float GetNumber(mytype depth);
public:
    Board MainBoard;
    float asses;
    Engine(mytype depth);
    ~Engine();
    mytype PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2);
    mytype EngineMove();
};

