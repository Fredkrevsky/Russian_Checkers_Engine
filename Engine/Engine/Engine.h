#pragma once

#include "AT.h"

//����� ��������� ������ ���� = 5.61224 * 10^(-6) ������

//���������: ����� ��������� ������ ����: 7,1 * 10^(-7) ������

//����� �������� ��� ������       AT
//1:   < 1 ��                     < 1 ��     
//2:   < 1 ��                     < 1 ��
//3:   1 - 5 ��                   < 1 ��
//4:   5 - 20 ��                  2 - 15 �� 
//5:   20 - 200 ��                5 - 30 ��
//6:   200 �� - 2 �               30 - 100 ��
//7:   2 - 15 �                   100 - 200 ��
//8:   15 � - 2 ��� 
//9:   2+ ���
//10:  12+ ���


enum MOVE_RESULT {
    INVALID_COORD,
    ONE_MORE,
    SUCCESS,
    WIN,
    DRAW,
    LOSE,
};

enum MOVE_ASSES {
    COMPULSORY,
    BRILLIANT,
    STRONGEST,
    NORMAL,
    MISTAKE,
    BLUNDER
};

class Engine {
private:
    mytype depth;
    bool turn;
    bool isActual;
    MOVE_TYPE type;
    mytype vector;
    mytype x, y;
    MOVE_ASSES getStatus(mytype index, float old);
    float oldasses;
    bool isSorted;
public:
    int nodes;
    Moves moves;
    int duration;
    TField field;
    float asses;
    Engine(mytype depth);
    ~Engine();
    MOVE_ASSES status;
    float accuracy;
    MOVE_RESULT PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2);
    MOVE_RESULT EngineMove();
};

