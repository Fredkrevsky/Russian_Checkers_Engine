#pragma once+
#include <SFML/Graphics.hpp>
#include "Engine.h"

using namespace sf;
#define schar signed char
typedef mytype Coord[4];

extern Font font;
const int fontSize = 24;
const int tileSize = 100;

class TLabel {
    Text text;
    bool visible;
public: 
    TLabel();
    void setText(std::string txt);
    void setPos(int x, int y);
    void draw(RenderWindow& win);
    void setVisible(bool toSet);
    void setThickness(int thick);
    void setFontSize(int fontSize);
    void setColor(Color color);
    void setOutlineColor(Color color);
};

class TObject
{
protected:
    RectangleShape background;
    int x, y, width, height;
    TObject();
    bool visible;
public:
    void setVisible(bool toSet);
    virtual ~TObject() {}
    virtual void setPos(int tx, int ty);
    virtual void setSize(int twidth, int theight);
    void setColor(Color color);
    void setThickness(int thickness);
    virtual void draw(RenderWindow& win);
};

class TClickable : public TObject {
protected:
    Vector2f pressPos;
    virtual void onPress() = 0;
    virtual void onRelease() = 0;
public:
    bool isPressed(Vector2f& pos);
    TClickable();
};

class TButton : public TClickable {
    Text text;
    int len;
    void normText();
public:
    TButton();
    void setText(std::string toSet);
    void setPos(int tx, int ty) override;
    void setSize(int twidth, int theight);
    void onPress();
    void onRelease();
    void draw(RenderWindow& win);
};

class TChoice : public TClickable {
    bool isSelected;
    RectangleShape in;
public:
    TChoice();
    void setPos(int tx, int ty) override;
    void setSize(int twidth, int theight);
    void onPress();
    void onRelease();
    void draw(RenderWindow& win);
    void setStatus(bool status);
    bool getStatus();
};

class TBar : public TObject {
protected:
    RectangleShape first;
    float value;
    int posX;
    RectangleShape second;
    Text text;

    virtual void setValue(float toSet) = 0;
    TBar();
public:
    void setFirstColor(Color color);
    void setSecondColor(Color color);
    void draw(RenderWindow& win);
};

class TProgressBar : public TBar {
    inline void setWidth();
    inline void setTextPosition();
    inline void setString();
public:
    TProgressBar();
    void setPos(int tx, int ty) override;
    void setSize(int twidth, int theight) override;
    void setValue(float toSet);
};

class TAssessBar : public TBar {

    inline void setHeight();
    inline void setTextColor();
    inline void setTextPosition();
    inline void setString();
    bool isFlip;
public:
    TAssessBar();
    void setPos(int tx, int ty) override;
    void setSize(int twidth, int theight) override;
    void setValue(float toSet);
    void flip();
};

class TCommentSection : TObject{
    std::vector<Text> vText;
    std::vector<char> values;
public:
    TCommentSection();
    void setPos(int x0, int y0) override;
    void setValues(std::vector<MoveData>& vdata);
    void draw(RenderWindow& win);
};

class TClock : public TObject {
    volatile bool gameIsGoing;
    volatile bool yourTurn;
    sf::Thread* thread;
    int value;
    Text text;
    std::string getStringTime(int seconds);
    void tictac();
public:
    TClock();
    void update(int seconds);
    void start();
    void pause();
    void draw(RenderWindow& win);
    void setPos(int tx, int ty) override;
    void release();
    void stop();
};

class TBoard : TObject {
    bool flipped;
    TField field = {};
    bool red[8][8] = {};
    mytype x1, y1, x2, y2;
    MOVE_STATUS comment;

    Texture forced, best, good, inac, blunder;
    bool isCaptured;
    mytype cx = -1, cy = -1;

public:
    void capture(int posx, int posy);
    void uncatch();
    void redReset();
    TBoard();
    void setPos(int x0, int y0) override;
    void setField(TField& toSet);
    void getCoord(Vector2f start, Vector2f end, mytype* coord);
    void redSet(mytype x1, mytype y1, mytype x2, mytype y2);
    void redReset(mytype x1, mytype y1, mytype x2, mytype y2);
    void flip();
    void draw(RenderWindow& win, int posx, int posy);
    void setComment(MOVE_STATUS comment, mytype x1, mytype y1, mytype x2, mytype y2);
};

class GameController {
protected:
    Engine engine;
    MOVE_TYPE type;
    mytype x, y, vector;

    void getData(MoveData& source);
    void setData(MoveData& dest);

public:
    TField field;
    float assess;
    int curr, head;
    bool turn;

    std::vector<MoveData> gameMoves;
    GameController();
    virtual MOVE_RESULT PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2);
    MOVE_RESULT EngineMove(mytype depth);
    void getPrev();
    void getNext();
    void getCurr();
};

class AnalysicsController {
    bool turn;

    Engine engine;
    MOVE_TYPE type;
    mytype x, y, vector;
    int curr, head;
    
    void getData(MoveData& source);

public:

    TField field;
    float assess;
    MOVE_STATUS comment;
    mytype x1, y1, x2, y2;

    std::vector<MoveData> gameMoves;
    AnalysicsController();
    void evaluate(int index, int depth);
    void setMoves(std::vector<MoveData>& tgameMoves);
    void getPrev();
    void getNext();
    void getCurr();
};

class TInput : public TClickable {
    bool isSelected;
    Text text;
    int limit;
    bool checkchar(char toCheck);
public:
    bool letters = false;
    bool dot = false;
    bool numbers = false;
    TInput();
    void onPress() override;
    void onKeyPress(char pressed);
    void onRelease() override;
    void draw(RenderWindow& win) override;
    void setPos(int x0, int y0) override;
    void setSize(int w, int h) override;
    void setLimit(int lim);
    std::string getText();
};

class TWait {

    const float s3 = 1.73205;

    CircleShape mas[6];
    int current = 0;
    int radius = 100;
    int x = 0, y = 0;
    bool visible = true;

    void setPos();
public:
    TWait();
    void setNext();
    void setPos(int tx, int ty);
    void setRadius(int tradius);
    void setVisible(bool toSet);
    void draw(RenderWindow& win);
};
