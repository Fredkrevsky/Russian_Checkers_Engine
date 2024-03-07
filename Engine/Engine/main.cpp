#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

using namespace sf;

const int DEPTH = 12;

Font font;
bool open = true;
bool turn = true;
mytype depth;

const int menuH = 100;
const int tileSize = 100;
const int leftW = 0;//(1920 - 8 * tileSize) / 2 - 100;
const int winH = tileSize * 8 + menuH;
const int winW = tileSize * 8 + 50;

class TChoise {
    RectangleShape field;
    RectangleShape in;
    int x, y;
public:
    bool isSelected;
    TChoise() {
        x = 0;
        y = 0;
        field.setSize(Vector2f(20, 20));
        field.setFillColor(Color::White);
        field.setOutlineColor(Color::Black);
        field.setOutlineThickness(2);

        isSelected = false;

        in.setSize(Vector2f(10, 10));
        in.setFillColor(Color::Black);

    }
    void setPos(int x0, int y0) {
        x = x0;
        y = y0;
        field.setPosition(x, y);
        in.setPosition(x + 5, y + 5);
    }
    bool isPressed(Vector2f pos) {
        return field.getGlobalBounds().contains(pos);
    }
    void draw(RenderWindow& win) {
        win.draw(field);
        if (isSelected) {
            win.draw(in);
        }
    }
};

class TLine {
    RectangleShape block;
    Text text;
    std::string result;
    std::string coord;
public:
    const int h = 50;
    TLine() {
        block.setFillColor(Color::White);
        block.setOutlineColor(Color::Black);
        block.setOutlineThickness(2);
        block.setSize(Vector2f(398, h - 2));

        text.setCharacterSize(24);
        text.setFillColor(Color::Black);
    }
    void setAsses(float asses) {
        if ((asses > 9) || (asses < -9)) {
            result = "win";
        }
        else if (abs(asses) < 0.1) {
            result = "0.0";
        }
        else {
            result = std::to_string(asses);
            mytype dot = result.find('.');
            result = result.substr(0, dot + 2);
        }
    }
    void setCoord(mytype* tcoord){
        mytype temp[4] = {};
        for (int i = 0; i < 4; i++) {
            temp[i] = tcoord[i];
        }
        coord = std::to_string(temp[0]) + " " + std::to_string(temp[1]) + " " + std::to_string(temp[2]) + " " + std::to_string(temp[3]);
    }
    void draw(RenderWindow& win, int x, int y) {
        block.setPosition(x + 2, y + 2);
        text.setPosition(52 + x, y + 2);
        text.setFont(font);
        text.setString(coord + "    " + result);
        win.draw(block);
        win.draw(text);
    }
};

class TMovesBlock {
    TLine* massiv;
    int len;
    int x;
    int y;
public:
    TMovesBlock() {
        massiv = nullptr;
        len = 0;
        x = 0;
        y = 0;
    }
    ~TMovesBlock() {
        if (massiv != nullptr) {
            delete[] massiv;
        }
    }
    void setMoves(Moves& moves) {
        len = std::min((int)moves.len, 8);
        if (len > 0) {
            massiv = new TLine[len];
            for (int i = 0; i < len; i++) {
                massiv[i].setCoord(moves.getCoord(i));
                massiv[i].setAsses(moves.getAsses());
            }
        }
    }
    void setPos(int x0, int y0) {
        x = x0;
        y = y0;
    }
    void draw(RenderWindow& win) {
        for (int i = 0; i < len; i++) {
            massiv[i].draw(win, x, y + massiv[i].h * i);
        }
    }
};

class TButton {
    Text text;
    RectangleShape rect;
    int h;
    int w;
    int x, y;
    int textx, texty;
public:
    TButton(){ 
        text.setFillColor(Color::Black);
        text.setCharacterSize(20);
        text.setFont(font);
        text.setPosition(0, 0);

        rect.setSize(Vector2f(w, h));
        rect.setOutlineColor(Color(30, 30, 30));
        rect.setOutlineThickness(2);
        rect.setPosition(0, 0);

        x = 0;
        y = 0;
        textx = 0;
        texty = 0;
        w = 100;
        h = 50;
    }
    void setSize(int sizex, int sizey) {
        w = sizex;
        h = sizey;
        rect.setSize(Vector2f(w, h));
    }
    void setColor(Color color) {
        rect.setFillColor(color);
    }
    void setString(std::string toSet) {
        text.setString(toSet);
    }
    void setPos(int x0, int y0, int tx0, int ty0) {
        x = x0;
        y = y0;
        textx = tx0;
        texty = ty0;

        rect.setPosition(x, y);
        text.setPosition(x + textx, y + texty);
    }
    void draw(RenderWindow& win) {
        win.draw(rect);
        win.draw(text);
    }
    bool isPressed(Vector2f& pos) {
        return rect.getGlobalBounds().contains(pos);
    }
};

class TBoard {
    int x;
    int y;
    bool flipped;
    TField field = {};
    bool red[8][8] = {};

    void redReset() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                red[i][j] = false;
            }
        }
    }
public:
    TBoard() {
        x = 0;
        y = 0;
        flipped = false;
    }
    void setField(TField toSet) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                field[i][j] = toSet[i][j];
            }
        }
        if (flipped) {
            flip();
            flipped = true;
        }
    }
    void setPos(int x0, int y0) {
        x = x0;
        y = y0;
    }
    MOVE_RESULT move(Engine& engine, Vector2f start, Vector2f end) {
        redReset();

        mytype x1 = (start.x - x) / tileSize;
        mytype y1 = (start.y - y) / tileSize;
        y1 = 7 - y1;

        mytype x2 = (end.x - x) / tileSize;
        mytype y2 = (end.y - y) / tileSize;
        y2 = 7 - y2;

        if (flipped) {
            x1 = 7 - x1;
            y1 = 7 - y1;
            x2 = 7 - x2;
            y2 = 7 - y2;
        }

        return engine.PlayerMove(x1, y1, x2, y2);
    }
    void redSet(Vector2f& start) {
        int x0 = (start.x - x) / tileSize;
        int y0 = (start.y - y) / tileSize;
        y0 = 7 - y0;

        if ((x0 >= 0) && (y0 >= 0) && (x0 < 8) && (y0 < 8) && ((x0 + y0) % 2 == 0)) {
            red[x0][y0] = !red[x0][y0];
        }
    }
    void flip() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 8; j++) {
                mytype temp = field[i][j];
                field[i][j] = field[7 - i][7 - j];
                field[7 - i][7 - j] = temp;
            }
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 8; j++) {
                bool temp = red[i][j];
                red[i][j] = red[7 - i][7 - j];
                red[7 - i][7 - j] = temp;
            }
        }
        flipped = !flipped;
    }
    void draw(RenderWindow& win) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                RectangleShape cell(Vector2f(tileSize, tileSize));
                if (red[i][j]) {
                    cell.setFillColor(Color(212, 109, 81));
                }
                else {
                    if ((i + j) % 2 == 1) {
                        cell.setFillColor(Color(233, 237, 204));
                    }
                    else {
                        cell.setFillColor(Color(119, 153, 84));
                    }
                }

                cell.setPosition(x + i * tileSize, y + (7 - j) * tileSize);
                win.draw(cell);

                if (field[i][j] != 0) {
                    CircleShape checker(tileSize / 2 - 10);
                    checker.setPosition(x + i * tileSize + 10, y + (7 - j) * tileSize + 10);

                    if ((field[i][j] == 1) || (field[i][j] == 3)) {
                        checker.setFillColor(Color(230, 230, 230));
                    }
                    else if ((field[i][j] == 2) || (field[i][j] == 4)) {
                        checker.setFillColor(Color(30, 30, 30));
                    }
                    win.draw(checker);
                    if (field[i][j] == 3) {
                        CircleShape in(tileSize / 4 - 5);
                        in.setPosition(x + i * tileSize + tileSize / 4 + 5, y + (7 - j) * tileSize + tileSize / 4 + 5);
                        in.setFillColor(Color(30, 30, 30));
                        win.draw(in);
                    }
                    else if (field[i][j] == 4) {
                        CircleShape in(tileSize / 4 - 5);
                        in.setPosition(x + i * tileSize + tileSize / 4 + 5, y + (7 - j) * tileSize + tileSize / 4 + 5);
                        in.setFillColor(Color(230, 230, 230));
                        win.draw(in);
                    }
                }
            }
        }
    }
};

class TAssessBar {
    int x;
    int y;
    int w;
    int h;
    int height;
    Text text;
    RectangleShape white;
    RectangleShape black;
    bool flipped;
    float assess;
    void setString() {
        float toSet = std::round(abs(assess) * 10) / 10;
        std::string result = "";
        if (toSet > 0.1) {
            if (toSet > 99) {
                result = "win";
            }
            else {
                result = std::to_string(toSet);
                mytype dot = result.find('.');
                result = result.substr(0, dot + 2);
            }
        }
        text.setString(result);
    }
    void setHeight() {
        if (abs(assess) < 0.1) {
            height = h / 2;
        }
        else if (assess > 5) {
            height = h;
        }
        else if (assess < -5) {
            height = 0;
        }
        else {
            height = h * (assess + 5) / 10;
        }
    }
    void setTextColor() {
        if (assess > 0) {
            text.setFillColor(Color(30, 30, 30));
        }
        else {
            text.setFillColor(Color(230, 230, 230));
        }
    }
    void setTextPosition() {
        if (flipped) {
            if (assess > 0) {
                text.setPosition(x + 16, y + 10);
            }
            else {
                text.setPosition(x + 12, y + h - 30);
            }
        }
        else {
            if (assess > 0) {
                text.setPosition(x + 16, y + h - 30);
            }
            else {
                text.setPosition(x + 12, y + 10);
            }
        }
    }
    void setBlocks() {
        white.setSize(Vector2f(w, height));
        black.setSize(Vector2f(w, h - height));
        if (flipped) {
            white.setPosition(x, y);
            black.setPosition(x, y + height);
        }
        else {
            black.setPosition(x, y);
            white.setPosition(x, y + h - height);
        }
    }
public:
    TAssessBar() {
        x = 0;
        y = 0;
        w = 50;
        h = 8 * tileSize;
        height = h / 2;        
        assess = 0.0;
        flipped = false;

        text.setFont(font);
        text.setCharacterSize(15);

        black.setSize(Vector2f(w, h - height));
        black.setFillColor(Color(30, 30, 30));

        white.setSize(Vector2f(w, height));
        white.setFillColor(Color(230, 230, 230));
    }
    void setAssess(float toSet) {

        assess = toSet;

        setString();
        setTextColor();
        setTextPosition();

        setHeight();
        setBlocks();

        
    }
    void setPos(int x0, int y0) {
        x = x0;
        y = y0;

        if (flipped) {
            black.setPosition(x, y + height);
            white.setPosition(x, y);
        }
        else {
            black.setPosition(x, y);
            white.setPosition(x, y + h - height);
        }
    }
    void flip() {
        if (flipped) {
            black.setPosition(x, y);
            white.setPosition(x, y + h - height);
        }
        else {
            black.setPosition(x, y + height);
            white.setPosition(x, y);
        }
        Vector2f temp = text.getPosition();
        text.setPosition(temp.x, y + h - (temp.y - y) - 20);
        flipped = !flipped;
    }
    void draw(RenderWindow& win) {
        win.draw(white);
        win.draw(black);
        win.draw(text);
    }
};

class TStartForm {

    RenderWindow& win;
    RectangleShape background;
    Text tChoise, whiteBText, blackBText;
    Text tDiff, easyBText, mediumBText, hardBText, impossibleBText;
    TChoise choise1, choise2, ch1, ch2, ch3, ch4;
    TButton startB, exitB;

    void draw() {
        win.clear();
        win.draw(background);
        win.draw(tChoise);
        win.draw(blackBText);
        win.draw(whiteBText);
        choise1.draw(win);
        choise2.draw(win);
        ch1.draw(win);
        ch2.draw(win);
        ch3.draw(win);
        ch4.draw(win);
        win.draw(tDiff);
        win.draw(easyBText);
        win.draw(mediumBText);
        win.draw(hardBText);
        win.draw(impossibleBText);
        startB.draw(win);
        exitB.draw(win);
        win.display();
    }
public:
    TStartForm(RenderWindow& renwin) : win(renwin) {

        background.setSize(Vector2f(win.getSize()));
        background.setFillColor(Color::White);

        tChoise.setString("Choose your color:");
        tChoise.setFont(font);
        tChoise.setCharacterSize(30);
        tChoise.setFillColor(Color::Black);
        tChoise.setPosition(300, 70);

        whiteBText.setString("White");
        whiteBText.setFont(font);
        whiteBText.setCharacterSize(30);
        whiteBText.setPosition(245, 130);
        whiteBText.setFillColor(Color::Black);

        blackBText.setString("Black");
        blackBText.setFont(font);
        blackBText.setCharacterSize(30);
        blackBText.setPosition(245, 180);
        blackBText.setFillColor(Color::Black);

        choise1.setPos(445, 140);
        choise2.setPos(445, 190);
        choise1.isSelected = true;
        turn = true;

        tDiff.setString("Choose the difficulty:");
        tDiff.setFont(font);
        tDiff.setCharacterSize(30);
        tDiff.setFillColor(Color::Black);
        tDiff.setPosition(289, 270);

        easyBText.setString("Easy");
        easyBText.setFont(font);
        easyBText.setCharacterSize(30);
        easyBText.setPosition(245, 330);
        easyBText.setFillColor(Color::Black);

        mediumBText.setString("Medium");
        mediumBText.setFont(font);
        mediumBText.setCharacterSize(30);
        mediumBText.setPosition(245, 380);
        mediumBText.setFillColor(Color::Black);

        hardBText.setString("Hard");
        hardBText.setFont(font);
        hardBText.setCharacterSize(30);
        hardBText.setPosition(245, 430);
        hardBText.setFillColor(Color::Black);

        impossibleBText.setString("Impossible");
        impossibleBText.setFont(font);
        impossibleBText.setCharacterSize(30);
        impossibleBText.setPosition(245, 480);
        impossibleBText.setFillColor(Color::Black);

        ch1.setPos(445, 340);
        ch2.setPos(445, 390);
        ch3.setPos(445, 440);
        ch4.setPos(445, 490);

        ch4.isSelected = true;
        depth = DEPTH;

        startB.setSize(125, 50);
        startB.setColor(Color::Green);
        startB.setString("Start");
        startB.setPos(270, 600, 40, 10);
        startB.draw(win);

        exitB.setSize(125, 50);
        exitB.setColor(Color::Green);
        exitB.setString("Exit");
        exitB.setPos(430, 600, 42, 10);
        exitB.draw(win);
    }
    void poll() {

        while (win.isOpen()) {
            Event event;

            while (win.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    win.close();
                    open = false;
                }
                else if (event.type == sf::Event::MouseButtonPressed) {
                    Vector2f pos = Vector2f(Mouse::getPosition(win));

                    if (choise1.isPressed(pos)) {
                        choise1.isSelected = true;
                        choise2.isSelected = false;
                        turn = true;
                    }
                    else if (choise2.isPressed(pos)) {
                        choise1.isSelected = false;
                        choise2.isSelected = true;
                        turn = false;
                    }
                    else if (ch1.isPressed(pos)) {
                        ch1.isSelected = true;
                        ch2.isSelected = false;
                        ch3.isSelected = false;
                        ch4.isSelected = false;
                        depth = 6;
                    }
                    else if (ch2.isPressed(pos)) {
                        ch1.isSelected = false;
                        ch2.isSelected = true;
                        ch3.isSelected = false;
                        ch4.isSelected = false;
                        depth = 8;
                    }
                    else if (ch3.isPressed(pos)) {
                        ch1.isSelected = false;
                        ch2.isSelected = false;
                        ch3.isSelected = true;
                        ch4.isSelected = false;
                        depth = 10;
                    }
                    else if (ch4.isPressed(pos)) {
                        ch1.isSelected = false;
                        ch2.isSelected = false;
                        ch3.isSelected = false;
                        ch4.isSelected = true;
                        depth = 12;
                    }
                    else if (startB.isPressed(pos)) {
                        win.close();
                        open = true;
                    }
                    else if (exitB.isPressed(pos)) {
                        win.close();
                        open = false;
                    }
                }
            }
            draw();
        }
    }
};

class TMainForm {
    bool flip;
    bool LP = false;
    bool LR = false;
    int duration;
    int num_nodes;

    RectangleShape background;
    Text dur;

    RenderWindow& win;
    TButton exitB;
    TBoard board;
    TAssessBar bar;
    TButton flipB;
    TMovesBlock block;
    Text nodes;

    void drawOther() {
        win.draw(background);
        if (duration != 0) {
            nodes.setString("kN/s = " + std::to_string((long)num_nodes / duration));
        }
        else {
            nodes.setString("N/s = 0");
        }
        dur.setString("Time = " + std::to_string(duration));
        win.draw(dur);
        win.draw(nodes);
    }
    void draw() {
        win.clear();
        drawOther();
        board.draw(win);
        bar.draw(win);
        exitB.draw(win);
        flipB.draw(win);
        block.draw(win);
        win.display();
    }
public:
    TMainForm(RenderWindow& renwin, Engine& engine, bool turn): win(renwin) {
        
        background.setSize(Vector2f(win.getSize()));
        background.setPosition(0, 0);
        background.setFillColor(Color::White);

        dur.setFont(font);
        dur.setCharacterSize(24);
        dur.setFillColor(Color::Black);
        dur.setPosition(100, 50);

        nodes.setFont(font);
        nodes.setCharacterSize(24);
        nodes.setFillColor(Color::Black);
        nodes.setPosition(400, 50);

        flip = !turn;

        exitB.setSize(200, 30);
        exitB.setColor(Color::Green);
        exitB.setString("Exit to main menu");
        exitB.setPos(1625, 10, 20, 3);

        board.setField(engine.field);
        board.setPos(leftW, menuH);

        bar.setPos(leftW + 8 * tileSize, menuH);
        bar.setAssess(engine.asses);

        flipB.setSize(100, 30);
        flipB.setColor(Color::Green);
        flipB.setString("Flip");
        flipB.setPos(50, 920, 30, 3);

        board.setField(engine.field);
        bar.setAssess(0.0);
        duration = 0;
        num_nodes = 0;

        if (!turn) {
            board.flip();
            bar.flip();

            draw();
            engine.EngineMove();
            board.setField(engine.field);
            bar.setAssess(engine.asses);
            duration = engine.duration;
            num_nodes = engine.nodes;
        }

        block.setMoves(engine.moves);
        block.setPos(leftW + tileSize * 8 + 50, 100);

        draw();

    }
    void poll(Engine& engine) {

        Vector2f LPPos, LRPos;

        while (win.isOpen())
        {
            Event event;

            while (win.pollEvent(event))
            {
                if (event.type == Event::Closed) {
                    win.close();
                    open = false;
                }
                if (event.type == Event::MouseButtonPressed) {
                    Vector2f pos = Vector2f(Mouse::getPosition(win));
                    if (exitB.isPressed(pos)) {
                        win.close();
                        open = true;
                    }
                    else if (flipB.isPressed(pos)) {
                        board.flip();
                        bar.flip();
                    }
                    else {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            LP = true;
                            LPPos = pos;
                        }
                        else if (event.mouseButton.button == sf::Mouse::Right) {
                            board.redSet(pos);
                        }
                    }
                }
                if (event.type == Event::MouseButtonReleased) {
                    if (LP) {
                        Vector2f pos = Vector2f(Mouse::getPosition(win));
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            LR = true;
                            LRPos = pos;
                        }
                    }
                }
                if (LP && LR) {
                    LP = false;
                    LR = false;

                    MOVE_RESULT result = board.move(engine, LPPos, LRPos);
                    board.setField(engine.field);
                    bar.setAssess(engine.asses);
                    draw();

                    if (result == SUCCESS) {
                        MOVE_RESULT result2 = engine.EngineMove();
                        board.setField(engine.field);
                        bar.setAssess(engine.asses);
                        duration = engine.duration;
                        num_nodes = engine.nodes;
                        block.setMoves(engine.moves);
                    }
                }
            }
            draw();
        }
    }
};

int main()
{

    Image icon;
    if (!icon.loadFromFile("Image/icon.png")) {
        return 1;
    }

    if (!font.loadFromFile("Fonts/arialmt.ttf")) {
        return 2;
    }

    while (open) {
        if (open) {
            RenderWindow start(VideoMode(850, 850), "VOBLA", Style::Close);
            start.setIcon(512, 512, icon.getPixelsPtr());
            start.setFramerateLimit(60);
            start.setVerticalSyncEnabled(true);
            
            TStartForm form(start);
            form.poll();
        }
        if (open) {
            RenderWindow main(VideoMode(1000, 1000), "VOBLA", Style::Close);
            main.setIcon(512, 512, icon.getPixelsPtr());
            main.setFramerateLimit(60);
            main.setVerticalSyncEnabled(true);
            
            Engine engine(depth);
            TMainForm form(main, engine, turn);
            form.poll(engine);
        }
    }

    return 0;
}