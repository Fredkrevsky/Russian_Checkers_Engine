#include "Engine.h"
#include <SFML/Graphics.hpp>
#include "controls.h"

bool open = true;
bool turn = true;

char depth;
const int DEPTH = 12;

Font font;
const int menuH = 100;
const int tileSize = 100;
const int leftW = 0;//(1920 - 8 * tileSize) / 2 - 100;
const int winH = tileSize * 8 + menuH;
const int winW = tileSize * 8 + 50;

class TStartForm {

    RenderWindow& win;
    RectangleShape background;
    Text tChoise, whiteBText, blackBText;
    Text tDiff, easyBText, mediumBText, hardBText, impossibleBText;
    TChoice choise1, choise2, ch1, ch2, ch3, ch4;
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
        choise1.setStatus(true);
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

        ch4.setStatus(true);
        depth = DEPTH;

        startB.setSize(125, 50);
        startB.setColor(Color::Green);
        startB.setText("Start");
        startB.setPos(270, 600);

        exitB.setSize(125, 50);
        exitB.setColor(Color::Green);
        exitB.setText("Exit");
        exitB.setPos(430, 600);

        draw();
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
                        choise1.setStatus(true);
                        choise2.setStatus(false);
                        turn = true;
                    }
                    else if (choise2.isPressed(pos)) {
                        choise1.setStatus(false);
                        choise2.setStatus(true);
                        turn = false;
                    }
                    else if (ch1.isPressed(pos)) {
                        ch1.setStatus(true);
                        ch2.setStatus(false);
                        ch3.setStatus(false);
                        ch4.setStatus(false);
                        depth = 6;
                    }
                    else if (ch2.isPressed(pos)) {
                        ch1.setStatus(false);
                        ch2.setStatus(true);
                        ch3.setStatus(false);
                        ch4.setStatus(false);
                        depth = 8;
                    }
                    else if (ch3.isPressed(pos)) {
                        ch1.setStatus(false);
                        ch2.setStatus(false);
                        ch3.setStatus(true);
                        ch4.setStatus(false);
                        depth = 10;
                    }
                    else if (ch4.isPressed(pos)) {
                        ch1.setStatus(false);
                        ch2.setStatus(false);
                        ch3.setStatus(false);
                        ch4.setStatus(true);
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

    RenderWindow& win;
    TButton exitB;
    TBoard board;
    TAssessBar bar;
    TButton flipB;

    GameController control;

    void drawOther() {
        win.draw(background);
    }
    void draw() {
        win.clear();
        drawOther();
        board.draw(win);
        bar.draw(win);
        exitB.draw(win);
        flipB.draw(win);
        win.display();
    }
public:
    TMainForm(RenderWindow& renwin, bool turn): win(renwin) {
        
        background.setSize(Vector2f(win.getSize()));
        background.setPosition(0, 0);
        background.setFillColor(Color::White);

        flip = !turn;

        exitB.setSize(200, 30);
        exitB.setColor(Color::Green);
        exitB.setText("Exit to main menu");
        exitB.setPos(1625, 10);

        board.setField(control.field);
        board.setPos(leftW, menuH);

        bar.setPos(leftW + 8 * tileSize, menuH);
        bar.setValue(control.assess);

        flipB.setSize(100, 30);
        flipB.setColor(Color::Green);
        flipB.setText("Flip");
        flipB.setPos(50, 920);

        board.setField(control.field);
        bar.setValue(0.0);
        duration = 0;
        num_nodes = 0;

        if (!turn) {
            board.flip();
            bar.flip();

            draw();
            control.EngineMove(depth);
            board.setField(control.field);
            bar.setValue(control.assess);
            num_nodes = 0;
        }
        draw();
    }
    void poll() {

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
                        if (event.mouseButton.button == Mouse::Left) {
                            LP = true;
                            LPPos = pos;
                        }
                        else if (event.mouseButton.button == Mouse::Right) {
                            board.redSet(pos);
                        }
                    }
                }
                if (event.type == Event::MouseButtonReleased) {
                    if (LP) {
                        Vector2f pos = Vector2f(Mouse::getPosition(win));
                        if (event.mouseButton.button == Mouse::Left) {
                            LR = true;
                            LRPos = pos;
                        }
                    }
                }
                if (Keyboard::isKeyPressed(Keyboard::Up)) {
                    control.getCurr();
                    board.setField(control.field);
                    bar.setValue(control.assess);
                }
                if (Keyboard::isKeyPressed(Keyboard::Left)) {
                    control.getPrev();
                    board.setField(control.field);
                    bar.setValue(control.assess);
                }
                if (Keyboard::isKeyPressed(Keyboard::Right)) {
                    control.getNext();
                    board.setField(control.field);
                    bar.setValue(control.assess);
                }
                if (LP && LR) {
                    LP = false;
                    LR = false;

                    mytype coord[4];
                    board.getCoord(LPPos, LRPos, coord);
                    MOVE_RESULT result = control.PlayerMove(coord[0], coord[1], coord[2], coord[3]);
                    if (result != INVALID_COORD) {
                        board.setField(control.field);
                        bar.setValue(control.assess);
                        draw();
                        if (result == SUCCESS) {
                            MOVE_RESULT result2 = control.EngineMove(depth);
                            board.setField(control.field);
                            bar.setValue(control.assess);
                            num_nodes = 0;
                        }
                    }
                }
            }
            draw();
        }
    }
};

class TAnalysicsForm {
public:
    TAnalysicsForm() {

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
            
            TMainForm form(main, turn);
            form.poll();
        }
    }

    return 0;
}