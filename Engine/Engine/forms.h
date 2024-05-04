#pragma once
#include <SFML/Network.hpp>
#include "controls.h"

const int menuH = 100;
const int winH = 1400;
const int winW = 1000;
const int leftW = 200;

extern Font font;
extern Image icon;

extern bool open;
extern bool turn;
extern bool mode;
extern bool pvp;
extern int depth;
extern TcpSocket socket;
extern TcpListener listener;

class TAnalysicsForm {
    RectangleShape background;
    RenderWindow& win;
    TButton exitB;
    TBoard board;
    TAssessBar bar;
    TButton flipB;
    TProgressBar pbar;
    AnalysicsController control;
    TCommentSection section;

    void drawprogress();
    void draw();
public:
    TAnalysicsForm(RenderWindow& renwin, std::vector<MoveData>& data);
    void poll();
};

class TStartForm {
    RenderWindow win;
    RectangleShape background;
    std::vector<TLabel> vLabel;
    std::vector<TChoice> vChoice;
    std::vector<TInput> vInput;
    TButton startB, exitB;
    int masDepth[4] = { 6, 8, 10, 12 };

    void draw();
public:
    TStartForm();
    void poll();
};

class TEngineForm {
    bool LP = false;
    bool LR = false;

    RenderWindow win;
    RectangleShape background;
    TButton exitB, flipB, analysicsB;
    TBoard board;
    TLabel x, y, vector, assess, coord, type, turnl;
    GameController control;

    void setText(int index);
    void draw(int posx, int posy);
public:
    TEngineForm();
    void poll();
};

class TPvpForm {
    bool turn = true;
    bool LP = false;
    bool LR = false;
    bool opponentMoveReceived = false;
    bool connected = false;

    std::vector<int> vMoves;

    TClock clock1, clock2;
    RectangleShape background;
    RenderWindow win;
    TButton exitB, flipB, analysicsB;
    TBoard board;
    TWait wait;
    GameController control;

    enum Type {
        INIT,
        MOVEREQ,
        MOVEREPLY,
    };

    void addMove(mytype x1, mytype y1, mytype x2, mytype y2);
    void sendMove(mytype x1, mytype y1, mytype x2, mytype y2);
    void receive();
    void draw();
    void loading();
public:
    TPvpForm();
    void poll();
};
