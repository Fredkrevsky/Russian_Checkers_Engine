#pragma once
#include <SFML/Graphics.hpp>
#include "Engine.h"

using namespace sf;
#define schar signed char
typedef mytype Coord[4];

extern Font font;



class TObject
{
protected:
    RectangleShape background;
    int x, y, width, height;
    TObject();
public:
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

class TBoard {
    int x;
    int y;
    bool flipped;
    TField field = {};
    bool red[8][8] = {};
    void redReset();
public:
    TBoard();
    void setField(TField toSet);
    void setPos(int x0, int y0);
    void getCoord(Vector2f start, Vector2f end, mytype* coord);
    void redSet(Vector2f& start);


    void flip();
    void draw(RenderWindow& win);
};

class GameController {
    Engine engine;
    bool turn;
    int curr, head;

    MOVE_TYPE type;
    mytype x, y, vector;

public:
    TField field;
    float assess;
    std::vector<MoveData> gameMoves;
    GameController() {
        turn = true;
        curr = 0;
        head = 0;
        assess = 0;
        BInit(field);

        MoveData temp;
        BCopy(temp.field, field);
        temp.assess = 0;
        temp.coord[0] = 0;
        temp.coord[1] = 0;
        temp.coord[2] = 0;
        temp.coord[3] = 0;
        temp.type = MOVE;
        temp.vector = 0;

        gameMoves.push_back(temp);
    }
    MOVE_RESULT PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2) {
        if (curr == head) {
            MoveData data;
            data.assess = 0;
            data.coord[0] = x1;
            data.coord[1] = y1;
            data.coord[2] = x2;
            data.coord[3] = y2;
            data.x = x;
            data.y = y;

            BCopy(data.field, field);
            data.type = type;
            data.vector = vector;

            MOVE_RESULT result = engine.PlayerMove(&data, turn);
            if (result != INVALID_COORD) {
                BCopy(field, data.field);
                type = data.type;
                vector = data.vector;
                x = data.coord[2];
                y = data.coord[3];
                gameMoves.push_back(data);
                head++;
                curr++;
                if (result == SUCCESS) {
                    turn = !turn;
                }
            }
            return result;
        }
    }
    MOVE_RESULT EngineMove(mytype depth) {
        getCurr();

        MoveData data;
        data.assess = 0;
        data.x = x;
        data.y = y;
        BCopy(data.field, field);
        data.type = type;
        data.vector = vector;

        MOVE_RESULT result = engine.EngineMove(&data, turn, depth);
        if (result == SUCCESS) {

            gameMoves.push_back(data);
            curr++;
            head++;

            assess = data.assess;
            BCopy(field, data.field);
            type = data.type;
            vector = data.vector;
            x = data.coord[2];
            y = data.coord[3];
            turn = !turn;
        }
        return result;
    }
    void getPrev() {
        if (curr > 0) {
            curr--;
            MoveData temp = gameMoves[curr];
            BCopy(field, temp.field);
            assess = temp.assess;
        }
    }
    void getNext() {
        if (curr < head) {
            curr++;
            MoveData temp = gameMoves[curr];
            BCopy(field, temp.field);
            assess = temp.assess;
        }
    }
    void getCurr() {
        if (curr != head) {
            curr = head;
            MoveData temp = gameMoves[curr];
            BCopy(field, temp.field);
            assess = temp.assess;
        }
    }
};