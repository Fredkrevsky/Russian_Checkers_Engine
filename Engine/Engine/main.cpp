#include "Engine.h"
#include <SFML/Graphics.hpp>
#include "controls.h"

//#define PVP

bool open = true;
bool turn = true;
bool mode = false;

int depth = 12;

Font font;

const int menuH = 100;
const int winH = 1400;
const int winW = 1000;
const int leftW = 200;

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


    void drawprogress() {
        win.clear();
        win.draw(background);
        board.draw(win);
        bar.draw(win);
        exitB.draw(win);
        flipB.draw(win);
        pbar.draw(win);
        section.draw(win);
        win.display();
    }
    void draw() {
        win.clear();
        win.draw(background);
        bar.draw(win);
        board.draw(win);
        exitB.draw(win);
        flipB.draw(win);
        section.draw(win);
        win.display();
    }
public:
    TAnalysicsForm(RenderWindow& renwin, std::vector<MoveData>& data) : win(renwin) {

        control.setMoves(data);
        background.setSize(Vector2f(win.getSize()));
        background.setFillColor(Color::White);

        exitB.setSize(200, 30);
        exitB.setColor(Color::Green);
        exitB.setText("Exit to main menu");
        exitB.setPos(1625, 10);

        board.setField(control.gameMoves[0].field);
        board.setPos(leftW, menuH);

        bar.setSize(46, 8 * tileSize - 4);
        bar.setPos(leftW + 8 * tileSize + 2, menuH + 2);
        bar.setValue(0);
        bar.setThickness(2);

        pbar.setSize(600, 50);
        pbar.setPos(300, 400);
        pbar.setThickness(2);
        pbar.setValue(0);

        flipB.setSize(100, 30);
        flipB.setColor(Color::Green);
        flipB.setText("Flip");
        flipB.setPos(50, 920);

        section.setPos(1100, 100);

        bar.setValue(0.0);
        drawprogress();
        
        for (int i = 1; i < control.gameMoves.size(); i++) {
            control.evaluate(i, 10);
            pbar.setValue((float)i / control.gameMoves.size());
            drawprogress();
        }
        section.setValues(control.gameMoves);
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
                else if (event.type == Event::MouseButtonPressed) {
                    Vector2f pos = Vector2f(Mouse::getPosition(win));
                    if (exitB.isPressed(pos)) {
                        win.close();
                        open = true;
                    }
                    else if (flipB.isPressed(pos)) {
                        board.flip();
                        bar.flip();
                    }
                }
                else if (Keyboard::isKeyPressed(Keyboard::Up)) {
                    control.getCurr();
                    board.setField(control.field);
                    board.setComment(control.comment, control.x1, control.y1, control.x2, control.y2);
                    bar.setValue(control.assess);
                }
                else if (Keyboard::isKeyPressed(Keyboard::Left)) {
                    control.getPrev();
                    board.setField(control.field);
                    board.setComment(control.comment, control.x1, control.y1, control.x2, control.y2);
                    bar.setValue(control.assess);
                }
                else if (Keyboard::isKeyPressed(Keyboard::Right)) {
                    control.getNext();
                    board.setField(control.field);
                    board.setComment(control.comment, control.x1, control.y1, control.x2, control.y2);
                    bar.setValue(control.assess);
                }
            }
            draw();
        }
    }
};

class TStartForm {
    RenderWindow& win;
    RectangleShape background;
    std::vector<TLabel> vLabel;
    std::vector<TChoice> vChoice;
    TButton startB, exitB;
    int masDepth[4] = { 6, 8, 10, 12 };

    void draw() {
        win.clear();
        win.draw(background);
        for (TLabel& elem : vLabel) {
            elem.draw(win);
        }
        for (TChoice& elem : vChoice) {
            elem.draw(win);
        }
        startB.draw(win);
        exitB.draw(win);
        win.display();
    }
public:
    TStartForm(RenderWindow& renwin) : win(renwin) {
        background.setSize(Vector2f(win.getSize()));
        background.setFillColor(Color::White);

        TLabel tempL;
        tempL.setText("Choose mode:");
        tempL.setPos(320, 30);
        vLabel.push_back(tempL);

        tempL.setText("vs Player:");
        tempL.setPos(245, 90);
        vLabel.push_back(tempL);

        tempL.setText("vs Engine:");
        tempL.setPos(245, 140);
        vLabel.push_back(tempL);

        tempL.setText("Choose your color:");
        tempL.setPos(300, 220);
        vLabel.push_back(tempL);

        tempL.setText("White");
        tempL.setPos(245, 280);
        vLabel.push_back(tempL);

        tempL.setText("Black");
        tempL.setPos(245, 330);
        vLabel.push_back(tempL);

        tempL.setText("Choose the difficulty:");
        tempL.setPos(289, 410);
        tempL.setVisible(false);
        vLabel.push_back(tempL);

        tempL.setText("Easy");
        tempL.setPos(245, 470);
        vLabel.push_back(tempL);

        tempL.setText("Medium");
        tempL.setPos(245, 520);
        vLabel.push_back(tempL);

        tempL.setText("Hard");
        tempL.setPos(245, 570);
        vLabel.push_back(tempL);

        tempL.setText("Impossible");
        tempL.setPos(245, 620);
        vLabel.push_back(tempL);

        TChoice tempC;
        tempC.setPos(445, 97);
        tempC.setStatus(true);
        vChoice.push_back(tempC);

        tempC.setPos(445, 147);
        tempC.setStatus(false);
        vChoice.push_back(tempC);

        turn = true;

        tempC.setPos(445, 285);
        tempC.setStatus(true);
        vChoice.push_back(tempC);
        tempC.setStatus(false);
        tempC.setPos(445, 335);
        vChoice.push_back(tempC);

        tempC.setVisible(false);
        tempC.setPos(445, 475);
        vChoice.push_back(tempC);
        tempC.setPos(445, 525);
        vChoice.push_back(tempC);
        tempC.setPos(445, 575);
        vChoice.push_back(tempC);
        tempC.setPos(445, 625);
        tempC.setStatus(true);
        vChoice.push_back(tempC);

        depth = masDepth[3];

        startB.setSize(125, 50);
        startB.setColor(Color::Green);
        startB.setText("Start");
        startB.setPos(240, 720);
        startB.setThickness(2);

        exitB.setSize(125, 50);
        exitB.setColor(Color::Green);
        exitB.setText("Exit");
        exitB.setPos(400, 720);
        exitB.setThickness(2);

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

                    int index = -1;
                    for (int i = 0; i < 8; i++) {
                        if (vChoice[i].isPressed(pos)) {
                            index = i;
                            break;
                        }
                    }
                    if (index != -1) {
                        if (index == 0) {
                            mode = false;
                            vChoice[0].setStatus(true);
                            vChoice[1].setStatus(false);
                            for (int i = 4; i < 8; ++i) {
                                vChoice[i].setVisible(false);
                            }
                            for (int i = 6; i < 11; ++i) {
                                vLabel[i].setVisible(false);
                            }
                        }
                        else if (index == 1) {
                            mode = true;
                            vChoice[1].setStatus(true);
                            vChoice[0].setStatus(false);
                            for (int i = 4; i < 8; ++i) {
                                vChoice[i].setVisible(true);
                            }
                            for (int i = 6; i < 11; ++i) {
                                vLabel[i].setVisible(true);
                            }
                        }
                        else if (index == 2) {
                            turn = true;
                            vChoice[2].setStatus(true);
                            vChoice[3].setStatus(false);
                        }
                        else if (index == 3) {
                            turn = false;
                            vChoice[2].setStatus(false);
                            vChoice[3].setStatus(true);
                        }
                        else {
                            for (int i = 4; i < 8; i++) {
                                if (i != index) {
                                    vChoice[i].setStatus(false);
                                }
                                else {
                                    vChoice[i].setStatus(true);
                                }
                            }
                            depth = masDepth[index - 4];
                        }
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
    bool LP = false;
    bool LR = false;

    RectangleShape background;

    RenderWindow& win;
    TButton exitB, flipB, analysicsB;
    TBoard board;

    TLabel x, y, vector, assess, coord, type, turnl;

    GameController control;

    void setText(int index) {
        if (control.gameMoves.size() > index) {
            MoveData curr;
            curr = control.gameMoves[index];
            x.setText("x = " + std::to_string(curr.x));
            y.setText("y = " + std::to_string(curr.y));
            vector.setText("vector = " + std::to_string(curr.vector));
            if (curr.type) {
                type.setText("type = BEAT");
            }
            else {
                type.setText("type = MOVE");
            }
            assess.setText("assess = " + std::to_string(curr.assess));
            coord.setText("coord = " + std::to_string(curr.coord[0]) + std::to_string(curr.coord[1]) + std::to_string(curr.coord[2]) + std::to_string(curr.coord[3]));
            if (curr.turn) {
                turnl.setText("White turn");
            }
            else {
                turnl.setText("Black turn");
            }
        }
    }

    void draw() {
        win.clear();
        win.draw(background);
        board.draw(win);
        exitB.draw(win);
        flipB.draw(win);
        analysicsB.draw(win);

        setText(control.curr);
        x.draw(win);
        y.draw(win);
        vector.draw(win);
        assess.draw(win);
        coord.draw(win);
        type.draw(win);
        turnl.draw(win);

        win.display();
    }
public:
    TMainForm(RenderWindow& renwin, bool turn): win(renwin) {
        
        background.setSize(Vector2f(win.getSize()));
        background.setFillColor(Color::White);

        x.setPos(1100, 250);
        x.setText("x = 0");
        y.setPos(1100, 300);
        y.setText("y = 0");
        vector.setPos(1100, 350);
        vector.setText("vector = 0");
        assess.setPos(1100, 400);
        assess.setText("assess = 0");
        coord.setPos(1100, 450);
        coord.setText("coord = 0");
        type.setPos(1100, 500);
        type.setText("type = 0");
        turnl.setPos(1100, 550);
        turnl.setText("turn = true");

        exitB.setSize(230, 60);
        exitB.setThickness(2);
        exitB.setColor(Color::Green);
        exitB.setText("Exit to main menu");
        exitB.setPos(1100, 700);

        board.setField(control.field);
        board.setPos(leftW, menuH);

        flipB.setSize(100, 30);
        flipB.setColor(Color::Green);
        flipB.setText("Flip");
        flipB.setPos(50, 920);

        analysicsB.setSize(200, 60);
        analysicsB.setColor(Color::Green);
        analysicsB.setText("Analysics");
        analysicsB.setPos(1100, 100);
        analysicsB.setThickness(2);


        board.setField(control.field);

        if (!turn) {
            board.flip();

            draw();
            if (mode){
                control.EngineMove(depth);
            }
            board.setField(control.field);
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
                else if (event.type == Event::MouseButtonPressed) {
                    Vector2f pos = Vector2f(Mouse::getPosition(win));
                    if (exitB.isPressed(pos)) {
                        win.close();
                        open = true;
                    }
                    else if (flipB.isPressed(pos)) {
                        board.flip();
                    }
                    else if (analysicsB.isPressed(pos)) {
                        RenderWindow start(VideoMode(1400, 1000), "VOBLA", Style::Close);
                        start.setFramerateLimit(60);
                        start.setVerticalSyncEnabled(true);

                        TAnalysicsForm form1(start, control.gameMoves);
                        form1.poll();
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
                else if (event.type == Event::MouseButtonReleased) {
                    if (LP) {
                        Vector2f pos = Vector2f(Mouse::getPosition(win));
                        if (event.mouseButton.button == Mouse::Left) {
                            LR = true;
                            LRPos = pos;
                        }
                    }
                }
                else if (Keyboard::isKeyPressed(Keyboard::Up)) {
                    control.getCurr();
                    setText(control.curr);
                    board.setField(control.field);
                }
                else if (Keyboard::isKeyPressed(Keyboard::Left)) {
                    control.getPrev();
                    setText(control.curr);
                    board.setField(control.field);
                }
                else if (Keyboard::isKeyPressed(Keyboard::Right)) {
                    control.getNext();
                    setText(control.curr);
                    board.setField(control.field);
                }
                if (LP && LR) {
                    LP = false;
                    LR = false;

                    mytype coord[4];
                    board.getCoord(LPPos, LRPos, coord);
                    MOVE_RESULT result = control.PlayerMove(coord[0], coord[1], coord[2], coord[3]);
                    if (result != INVALID_COORD) {
                        board.setField(control.field);
                        if (!mode){
                            if (result == SUCCESS) {
                                board.flip();
                            }
                        }
                        else {
                            draw();
                            if (result == SUCCESS) {
                                do {
                                    result = control.EngineMove(depth);
                                    board.setField(control.field);
                                } while (result == ONE_MORE);
                            }
                        }
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
            RenderWindow start(VideoMode(760, 850), "VOBLA", Style::Close);
            start.setIcon(512, 512, icon.getPixelsPtr());
            start.setFramerateLimit(60);
            start.setVerticalSyncEnabled(true);
            
            TStartForm form(start);
            form.poll();
        }
        if (open) {
            RenderWindow main(VideoMode(winH, winW), "VOBLA", Style::Close);
            main.setIcon(512, 512, icon.getPixelsPtr());
            main.setFramerateLimit(60);
            main.setVerticalSyncEnabled(true);
            
            TMainForm form(main, turn);
            form.poll();
        }
    }

    return 0;
}