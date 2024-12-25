#pragma once
#include <SFML/Network.hpp>
#include "controls.h"
#include <array>
#include <mutex>

extern Font font;
extern Image icon;

extern bool open;
extern bool turn;
extern bool pvp;
extern int depth;
//extern TcpSocket socket;
//extern TcpListener listener;

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
    const std::array<int, 4> masDepth =  { 4, 8, 10, 12 };

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
    TLabel resultLabel, timeLabel;
    TBoard board;
    GameController control;
    std::thread* engineThread;
    std::mutex labelMutex;

    void draw(int posx, int posy);
    void engineMove();
public:
    TEngineForm();
    ~TEngineForm();
    void poll();
};

class TPvpForm {
    bool turn = true;
    bool LP = false;
    bool LR = false;
    bool connected = false;

    std::vector<int> vMoves;

    TClock clock1, clock2;
    RectangleShape background;
    RenderWindow win;
    TButton exitB, flipB, analysicsB, drawB, resignB;
    TBoard board;
    TWait wait;
    GameController control;
    TLabel lDraw, lLose, lWin;

    enum Type {
        INIT,
        MOVEREQ,
        DRAWREQ,
        RESIGNREQ
    };

    void resign();
    void offerdraw();
    void sendMove(mytype x1, mytype y1, mytype x2, mytype y2);
    void receive();
    void draw(int posx, int posy);
    void loading();
public:
    TPvpForm();
    void poll();
};
