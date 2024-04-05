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
    bool visible;
public: 
    TLabel();
    void setText(std::string txt);
    void setPos(int x, int y);
    void draw(RenderWindow& win);
    void setVisible(bool toSet);
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
    TCommentSection() : TObject() {
        setThickness(2);
        for (int i = 0; i < 10; ++i) {
            values.push_back(0);
        }

        Text text;
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setFillColor(Color::Black);
        text.setString("Results:");
        text.setPosition(x + 50, y);
        vText.push_back(text);

        text.setString("0");
        text.setPosition(x, y + 50);
        vText.push_back(text);
        text.setString("Best");
        text.setPosition(x + 30, y + 50);
        vText.push_back(text);
        text.setString("0");
        text.setPosition(x + 200, y + 50);
        vText.push_back(text);

        text.setString("0");
        text.setPosition(x, y + 100);
        vText.push_back(text);
        text.setString("Good");
        text.setPosition(x + 30, y + 100);
        vText.push_back(text);
        text.setString("0");
        text.setPosition(x + 200, y + 100);
        vText.push_back(text);

        text.setString("0");
        text.setPosition(x, y + 150);
        vText.push_back(text);
        text.setString("Forced");
        text.setPosition(x + 30, y + 150);
        vText.push_back(text);
        text.setString("0");
        text.setPosition(x + 200, y + 150);
        vText.push_back(text);

        text.setString("0");
        text.setPosition(x, y + 200);
        vText.push_back(text);
        text.setString("Inaccuracy");
        text.setPosition(x + 30, y + 200);
        vText.push_back(text);
        text.setString("0");
        text.setPosition(x + 200, y + 200);
        vText.push_back(text);

        text.setString("0");
        text.setPosition(x, y + 250);
        vText.push_back(text);
        text.setString("Blunder");
        text.setPosition(x + 30, y + 250);
        vText.push_back(text);
        text.setString("0");
        text.setPosition(x + 200, y + 250);
        vText.push_back(text);

        text.setPosition(x, y + 320);
        text.setString("White accuracy:");
        vText.push_back(text);
        text.setPosition(x + 200, y + 320);
        text.setString("0.0%");
        vText.push_back(text);

        text.setPosition(x, y + 370);
        text.setString("Black accuracy:");
        vText.push_back(text);
        text.setPosition(x + 200, y + 370);
        text.setString("0.0%");
        vText.push_back(text);
    }
    void setPos(int x0, int y0) override {
        TObject::setPos(x0, y0);

        vText[0].setPosition(x0 + 50, y0);
        vText[1].setPosition(x0, y0 + 50);
        vText[2].setPosition(x0 + 30, y0 + 50);
        vText[3].setPosition(x0 + 200, y0 + 50);

        vText[4].setPosition(x0, y0 + 100);
        vText[5].setPosition(x0 + 30, y0 + 100);
        vText[6].setPosition(x0 + 200, y0 + 100);

        vText[7].setPosition(x0, y0 + 150);
        vText[8].setPosition(x0 + 30, y0 + 150);
        vText[9].setPosition(x0 + 200, y0 + 150);

        vText[10].setPosition(x0, y0 + 200);
        vText[11].setPosition(x0 + 30, y0 + 200);
        vText[12].setPosition(x0 + 200, y0 + 200);

        vText[13].setPosition(x0, y0 + 250);
        vText[14].setPosition(x0 + 30, y0 + 250);
        vText[15].setPosition(x0 + 200, y0 + 250);

        vText[16].setPosition(x0, y0 + 320);
        vText[17].setPosition(x0 + 200, y0 + 320);
        vText[18].setPosition(x0, y0 + 370);
        vText[19].setPosition(x0 + 200, y0 + 370);
        
    }
    void setValues(std::vector<MoveData>& vdata) {
        int white, black, sumwhite, sumblack;
        sumblack = sumwhite = white = black = 0;
        for (int i = 1; i < vdata.size(); ++i) {
            switch (vdata[i].comment) {
            case BEST: 
                if (vdata[i].turn) {
                    values[0]++;
                    sumwhite += 10;
                    white += 10;
                }
                else {
                    values[1]++;
                    sumblack += 10;
                    black += 10;
                }
                break;
            case GOOD: 
                if (vdata[i].turn) {
                    values[2]++;
                    sumwhite += 10;
                    white += 10;
                }
                else {
                    values[3]++;
                    sumblack += 10;
                    black += 10;
                }
                break;
            case FORCED: 
                if (vdata[i].turn) {
                    values[4]++;
                    sumwhite += 10;
                    white += 10;
                }
                else {
                    values[5]++;
                    sumblack += 10;
                    black += 10;
                }
                break;
            case INACCURACY: if (vdata[i].turn) {
                values[6]++;
                sumwhite += 10;
                white += 3;
            }
            else {
                values[7]++;
                sumblack += 10;
                black += 3;
            }
            break;
            case BLUNDER:
                if (vdata[i].turn) {
                    values[8]++;
                    sumwhite += 10;
                }
                else {
                    values[9]++;
                    sumblack += 10;
                }
                break;
            }
        }
        for (int i = 0; i < 5; ++i) {
            vText[3 * i + 1].setString(std::to_string(values[2 * i]));
            vText[3 * i + 3].setString(std::to_string(values[2 * i + 1]));
        }
        if (sumwhite) {
            vText[17].setString(std::to_string(white * 100 / sumwhite) + "." + std::to_string((white * 1000 / sumwhite) % 10) + "%");
        }
        if (sumblack) {
            vText[19].setString(std::to_string(black * 100 / sumblack) + "." + std::to_string((black * 1000 / sumblack) % 10) + "%");
        }
    }
    void draw(RenderWindow& win) {
        TObject::draw(win);
        for (auto& elem : vText) {
            win.draw(elem);
        }
    }
};

class TBoard : TObject {
    bool flipped;
    TField field = {};
    bool red[8][8] = {};
    void redReset();
    mytype x1, y1, x2, y2;
    MOVE_STATUS comment;

    Texture forced, best, good, inac, blunder;

public:
    TBoard();
    void setPos(int x0, int y0) override;
    void setField(TField& toSet);
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

    void getData(MoveData& source);
    void setData(MoveData& dest);

public:
    TField field;
    float assess;
    int curr, head;

    std::vector<MoveData> gameMoves;
    GameController();
    MOVE_RESULT PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2);
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