#pragma once
#include <SFML/Graphics.hpp>
#include "Engine.h"
#include <array>
#include <string>
#include <thread>
#include <memory>

using namespace sf;
#define schar signed char
typedef mytype Coord[4];

extern Font font;
static const int fontSize = 24;
static const int tileSize = 100;

using std::vector, std::string, std::thread, std::unique_ptr, std::array;

class TLabel {
public: 
    TLabel();
    void setText(std::string txt);
    void setPos(int x, int y);
    void draw(RenderWindow& win) const;
    void setVisible(bool toSet);
    void setThickness(int thick);
    void setFontSize(int fontSize);
    void setColor(Color color);
    void setOutlineColor(Color color);

private:
    Text text;
    bool visible;
};

class TObject
{
public:
    void setVisible(bool toSet);
    virtual ~TObject() {}
    virtual void setPos(int tx, int ty);
    virtual void setSize(int twidth, int theight);
    void setColor(Color color);
    void setThickness(int thickness);
    virtual void draw(RenderWindow& win) const;

protected:
    RectangleShape background;
    int x, y, width, height;
    TObject();
    bool visible;

};

class TClickable : public TObject {
public:
    bool isPressed(Vector2f& pos);
    TClickable();

protected:
    Vector2f pressPos;
    virtual void onPress() = 0;
    virtual void onRelease() = 0;
};

class TButton : public TClickable {
public:
    TButton();
    void setText(std::string toSet);
    void setPos(int tx, int ty) override;
    void setSize(int twidth, int theight);
    void onPress();
    void onRelease();
    void draw(RenderWindow& win) const override;

private:
    Text text;
    int len;
    void normText();
};

class TChoice : public TClickable {
public:
    TChoice();
    void setPos(int tx, int ty) override;
    void setSize(int twidth, int theight);
    void onPress();
    void onRelease();
    void draw(RenderWindow& win) const override;
    void setStatus(bool status);
    bool getStatus();

private:
    bool isSelected;
    RectangleShape in;
};

class TBar : public TObject {
public:
    TBar();
    virtual void setValue(float toSet) = 0;
    void setFirstColor(Color color);
    void setSecondColor(Color color);
    void draw(RenderWindow& win) const override;

protected:
    RectangleShape first;
    float value;
    int posX;
    RectangleShape second;
    Text text;
};

class TProgressBar : public TBar {
public:
    TProgressBar();
    void setPos(int tx, int ty) override;
    void setSize(int twidth, int theight) override;
    void setValue(float toSet);

private:
    inline void setWidth();
    inline void setTextPosition();
    inline void setString();
};

class TAssessBar : public TBar {
public:
    TAssessBar();
    void setPos(int tx, int ty) override;
    void setSize(int twidth, int theight) override;
    void setValue(float toSet);
    void flip();

private:
    inline void setHeight();
    inline void setTextColor();
    inline void setTextPosition();
    inline void setString();
    bool isFlip;
};

class TCommentSection : TObject{
public:
    TCommentSection();
    void setPos(int x0, int y0) override;
    void setValues(std::vector<MoveData>& vdata);
    void draw(RenderWindow& win) const override;

private:
    vector<Text> vText;
    vector<char> values;
};

class TClock : public TObject {
public:
    TClock();
    void update(int seconds);
    void start();
    void pause();
    void draw(RenderWindow& win) const override;
    void setPos(int tx, int ty) override;
    void release();
    void stop();

private:
    bool gameIsGoing;
    bool yourTurn;
    unique_ptr<thread> thread;
    int value;
    Text text;
    string getStringTime(int seconds);
    void tictac();
};

class TBoard : TObject {
public:
    void capture(int posx, int posy);
    void uncatch();
    TBoard();
    void setPos(int x0, int y0) override;
    void setField(TField& toSet);
    void getCoord(Vector2f start, Vector2f end, mytype* coord);
    void flip();
    void draw(RenderWindow& win) const override;
    void setComment(MOVE_STATUS comment, mytype x1, mytype y1, mytype x2, mytype y2);

private:
    bool flipped {false};
    TField field{};
    mytype x1{}, y1{}, x2{}, y2{};
    MOVE_STATUS comment;

    Texture forced, best, good, inac, blunder;  //Make array
    bool isCaptured{false};
    mytype cx{ -1 }, cy{ -1 };

};

class GameController {
public:
    TField field;
    float assess;
    int curr, head;
    bool turn;

    vector<MoveData> gameMoves;
    GameController();
    MOVE_RESULT PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2);
    MOVE_RESULT EngineMove(mytype depth);
    void getPrev();
    void getNext();
    void getCurr();

protected:
    Engine engine;
    MOVE_TYPE type;
    mytype x, y, vec;
    bool locked = false;

    void getData(MoveData& source);
    void setData(MoveData& dest);

};

class AnalysicsController {
public:
    TField field;
    float assess;
    MOVE_STATUS comment;
    mytype x1, y1, x2, y2;

    vector<MoveData> gameMoves;
    AnalysicsController();
    void evaluate(int index, int depth);
    void setMoves(vector<MoveData>& tgameMoves);
    void getPrev();
    void getNext();
    void getCurr();

private:
    bool turn;

    Engine engine;
    MOVE_TYPE type;
    mytype x, y, vec;
    int curr, head;
    
    void getData(MoveData& source);
};

class TInput : public TClickable {
public:
    bool letters = false;
    bool dot = false;
    bool numbers = false;
    TInput();
    void onPress() override;
    void onKeyPress(char pressed);
    void onRelease() override;
    void draw(RenderWindow& win) const override;
    void setPos(int x0, int y0) override;
    void setSize(int w, int h) override;
    void setLimit(int lim);
    std::string getText();

private:
    bool isSelected;
    Text text;
    int limit;
    bool checkchar(char toCheck);
};

class TWait : TObject {
public:
    TWait();
    void setNext();
    void setPos(int tx, int ty);
    void setRadius(int tradius);
    void setVisible(bool toSet);
    void draw(RenderWindow& win) const override;

private:
    const float s3{ 1.73205f };
    array<CircleShape, 6> mas;
    int current = 0;
    int radius = 100;

    void setPos();
};
