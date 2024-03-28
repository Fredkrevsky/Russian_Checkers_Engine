#pragma once
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
public: 
    TLabel() {
        text.setString("");
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setFillColor(Color::Black);
        text.setPosition(0, 0);
    }
    void setText(std::string txt) {
        text.setString(txt);
    }
    void setPos(int x, int y) {
        text.setPosition(x, y);
    }
    void draw(RenderWindow& win) {
        win.draw(text);
    }
};

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
    mytype x1, y1, x2, y2;
    MOVE_STATUS comment;

    Texture forced, best, good, inac, blunder;

public:
    TBoard();
    void setField(TField& toSet);
    void setPos(int x0, int y0);
    void getCoord(Vector2f start, Vector2f end, mytype* coord);
    void redSet(Vector2f& start);
    void flip();
    void draw(RenderWindow& win);
    void setComment(MOVE_STATUS comment, mytype x1, mytype y1, mytype x2, mytype y2);
};

class GameController {
    Engine engine;
    bool turn;


    MOVE_TYPE type;
    mytype x, y, vector;

    void getData(MoveData& source) {
        assess = source.assess;
        BCopy(field, source.field);
        type = source.type;
        vector = source.vector;
        x = source.x;
        y = source.y;
    }
    void setData(MoveData& dest) {
        dest.assess = assess;
        dest.x = x;
        dest.y = y;
        BCopy(dest.field, field);
        BCopy(dest.oldfield, field);
        dest.turn = turn;
        dest.type = type;
        dest.vector = vector;
    }

public:

    TField field;
    float assess;
    int curr, head;

    std::vector<MoveData> gameMoves;
    GameController() {
        type = MOVE;
        x = y = vector = 0;
        turn = true;
        curr = 0;
        head = 0;
        assess = 0;
        BInit(field);

        MoveData temp;
        setData(temp);
        temp.coord[0] = 0;
        temp.coord[1] = 0;
        temp.coord[2] = 0;
        temp.coord[3] = 0;

        gameMoves.push_back(temp);
    }
    MOVE_RESULT PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2) {
        if (curr == head) {

            MoveData data;
            setData(data);
            data.coord[0] = x1;
            data.coord[1] = y1;
            data.coord[2] = x2;
            data.coord[3] = y2;

            MoveData temp = data;
            MOVE_RESULT result = engine.PlayerMove(data);
            if (result != INVALID_COORD) {
                BCopy(temp.field, data.field);
                gameMoves.push_back(temp);
                getData(data);
                if (result == SUCCESS) {
                    turn = !turn;
                }
                head++;
                curr++;
            }
            return result;
        }
    }
    MOVE_RESULT EngineMove(mytype depth) {
        getCurr();

        MoveData data;
        setData(data);

        MoveData temp = data;
        MOVE_RESULT result = engine.EngineMove(data, depth);
        if (result == ONE_MORE || result == SUCCESS) {
            BCopy(temp.field, data.field);
            temp.coord[0] = data.coord[0];
            temp.coord[1] = data.coord[1];
            temp.coord[2] = data.coord[2];
            temp.coord[3] = data.coord[3];
            gameMoves.push_back(temp);
            getData(data);
            if (result == SUCCESS) {
                turn = !turn;
            }
            curr++;
            head++;
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

class AnalysicsController {
    bool turn;

    Engine engine;
    MOVE_TYPE type;
    mytype x, y, vector;
    int curr, head;
    
    void getData(MoveData& source) {
        assess = source.assess;
        BCopy(field, source.field);
        type = source.type;
        vector = source.vector;
        x = source.x;
        y = source.y;
        x1 = source.coord[0];
        y1 = source.coord[1];
        x2 = source.coord[2];
        y2 = source.coord[3];
        comment = source.comment;
    }

public:

    TField field;
    float assess;
    MOVE_STATUS comment;
    mytype x1, y1, x2, y2;

    std::vector<MoveData> gameMoves;
    AnalysicsController() {

        x1 = x2 = y1 = y2 = 0;
        comment = FORCED;
        type = MOVE;
        x = y = vector = 0;
        turn = true;
        curr = 0;
        head = 0;
        assess = 0;
        BInit(field);

    }
    void evaluate(int index, int depth) {
        engine.evaluate(gameMoves[index], depth);
    }
    void setMoves(std::vector<MoveData>& tgameMoves) {
        gameMoves = tgameMoves;
    }
    void getPrev() {
        if (curr > 0) {
            curr--;
            getData(gameMoves[curr]);
        }
    }
    void getNext() {
        if (curr < gameMoves.size() - 1) {
            curr++;
            getData(gameMoves[curr]);
        }
    }
    void getCurr() {
        if (curr != head) {
            curr = head;
            getData(gameMoves[curr]);
        }
    }
};