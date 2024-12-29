#pragma once
#include <SFML/Graphics.hpp>
#include "Engine.h"
#include <array>
#include <string>
#include <thread>
#include <memory>

using namespace sf;

extern Font font;
static constexpr int fontSize = 24;
static constexpr int tileSize = 100;

using std::vector, std::string, std::thread, std::unique_ptr, std::array,
std::function;

class TObject {
public:
    void setVisible(bool _visible);
    virtual void setPosition(Vector2f position);
    virtual void setSize(Vector2f size);
    void setColor(Color color);
    void setThickness(float thickness);
    virtual void draw(RenderWindow& win) const;

protected:
    TObject();
    virtual ~TObject() {};

    RectangleShape background;
    Vector2f position{}, size{};
    bool visible{ true };
};

class TLabel final : public TObject {
public: 
    TLabel();
    TLabel(const string& _caption, const Vector2f _pos, bool _visible = true);

    void setText(const string& _text);
    void setPosition(Vector2f position) override;
    void draw(RenderWindow& win) const override;
    void setTextThickness(float thick);
    void setFontSize(int fontSize);
    void setTextColor(Color color);
    void setOutlineColor(Color color);

private:
    Text text;
};

class TClickable : public TObject {
public:
    TClickable();
    bool isPressed(Vector2i pos) const;
    void setOnPress(const function<void()>& callback);
    virtual void onPress();

protected:
    function<void()> onPressFunction;
};

class TButton final : public TClickable {
public:
    TButton();
    TButton(const string& _caption, Vector2f _pos, Vector2f _size);

    void setCaption(const string& _caption);
    void setPosition(Vector2f position) override;
    void setSize(Vector2f size);
    void onPress() override;
    void onRelease();
    void draw(RenderWindow& win) const override;

private:
    Text caption;
    void normCaption();
};

class TChoice final : public TClickable {
public:
    TChoice();
    TChoice(Vector2f _position, const function<void()>& callback, bool _status, bool _visible = true);

    void setPosition(Vector2f position) override;
    void setSize(Vector2f position) override;
    void onRelease();
    void draw(RenderWindow& win) const override;
    void setStatus(bool status);
    bool getStatus();

private:
    bool status;
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
    RectangleShape first, second;
    Text text;
    float value;
    float posX;
};

class TProgressBar final : public TBar {
public:
    TProgressBar();
    void setPosition(Vector2f position) override;
    void setSize(Vector2f size) override;
    void setValue(float toSet);

private:
    inline void setWidth();
    inline void setTextPosition();
    inline void setString();
};

class TAssessBar final : public TBar {
public:
    TAssessBar();
    void setPosition(Vector2f position) override;
    void setSize(Vector2f size) override;
    void setValue(float toSet);
    void flip();

private:
    inline void setHeight();
    inline void setTextColor();
    inline void setTextPosition();
    inline void setString();
    bool isFlip;
};

class TCommentSection final : TObject{
public:
    TCommentSection();
    void setPosition(Vector2f position) override;
    void setValues(vector<MoveData>& vdata);
    void draw(RenderWindow& win) const override;

private:
    vector<Text> vText;
    vector<char> values;
};

class TClock final : public TObject {
public:
    TClock();
    void update(int seconds);
    void start();
    void pause();
    void draw(RenderWindow& win) const override;
    void setPosition(Vector2f position) override;
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

class TBoard final : TObject {
public:
    void capture(int posx, int posy);
    void uncatch();
    TBoard();
    void setPosition(Vector2f position) override;
    void setField(TField& toSet);
    void getCoord(Vector2f start, Vector2f end, mytype coord[]);
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

class GameController final {
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

class AnalysicsController final {
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

class TInput final : public TClickable {
public:
    bool letters{ false };
    bool dot{ false };
    bool numbers{ false };
    TInput();
    void onPress() override;
    void onKeyPress(char pressed);
    void onRelease();
    void draw(RenderWindow& win) const override;
    void setPosition(Vector2f position) override;
    void setSize(Vector2f position) override;
    void setLimit(int lim);
    string getText();

private:
    bool isSelected;
    Text text;
    int limit;
    bool checkchar(char toCheck);
};

class TWait final : TObject {
public:
    TWait();
    void setNext();
    void setPosition(Vector2f _position) override;
    void setRadius(int tradius);
    void setVisible(bool toSet);
    void draw(RenderWindow& win) const override;

private:
    const float s3{ 1.73205f };
    array<CircleShape, 6> mas;
    int current{ 0 };
    int radius{ 100 };

    void setPos();
};
