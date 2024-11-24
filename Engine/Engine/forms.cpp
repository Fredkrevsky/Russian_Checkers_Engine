#include "forms.h"
#include <mutex>
#include <chrono>

void TAnalysicsForm::drawprogress() {
    win.clear();
    win.draw(background);
    board.draw(win, 0, 0);
    bar.draw(win);
    exitB.draw(win);
    flipB.draw(win);
    pbar.draw(win);
    section.draw(win);
    win.display();
}
void TAnalysicsForm::draw() {
    win.clear();
    win.draw(background);
    bar.draw(win);
    board.draw(win, 0, 0);
    exitB.draw(win);
    flipB.draw(win);
    section.draw(win);
    win.display();
}
TAnalysicsForm::TAnalysicsForm(RenderWindow& renwin, std::vector<MoveData>& data) : win(renwin) {

    control.setMoves(data);
    background.setSize(Vector2f(win.getSize()));
    background.setFillColor(Color::White);

    board.setField(control.gameMoves[0].field);
    board.setPos(100, 50);

    bar.setSize(46, 8 * tileSize - 4);
    bar.setPos(100 + 8 * tileSize + 2, 52);
    bar.setValue(0);
    bar.setThickness(2);

    pbar.setSize(600, 50);
    pbar.setPos(200, 400);
    pbar.setThickness(2);
    pbar.setValue(0);

    exitB.setSize(100, 50);
    exitB.setThickness(2);
    exitB.setColor(Color::Green);
    exitB.setText("Exit");
    exitB.setPos(1130, 800 - 2);

    flipB.setSize(100, 50);
    flipB.setColor(Color::Green);
    flipB.setText("Flip");
    flipB.setPos(1010, 800 - 2);
    flipB.setThickness(2);

    section.setPos(1000, 50);

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
void TAnalysicsForm::poll() {

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

void TStartForm::draw() {
    win.clear();
    win.draw(background);
    for (TLabel& elem : vLabel) {
        elem.draw(win);
    }
    for (TChoice& elem : vChoice) {
        elem.draw(win);
    }
    for (TInput& elem : vInput) {
        elem.draw(win);
    }
    startB.draw(win);
    exitB.draw(win);
    win.display();
}
TStartForm::TStartForm() : win(VideoMode(760, 850), "Russian checkers", Style::Close) {

    win.setIcon(512, 512, icon.getPixelsPtr());
    win.setFramerateLimit(60);
    win.setVerticalSyncEnabled(true);

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
    tempL.setVisible(false);
    vLabel.push_back(tempL);

    tempL.setText("White");
    tempL.setPos(245, 280);
    vLabel.push_back(tempL);

    tempL.setText("Black");
    tempL.setPos(245, 330);
    vLabel.push_back(tempL);

    tempL.setText("Choose the difficulty:");
    tempL.setPos(289, 410);
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

    tempL.setVisible(true);
    tempL.setPos(300, 425);
    tempL.setText("Enter server IP");
    vLabel.push_back(tempL);
    tempL.setPos(300, 525);
    tempL.setText("Enter server port");
    vLabel.push_back(tempL);


    TChoice tempC;
    tempC.setPos(445, 97);
    tempC.setStatus(true);
    vChoice.push_back(tempC);

    tempC.setPos(445, 147);
    tempC.setStatus(false);
    vChoice.push_back(tempC);

    turn = true;

    tempC.setVisible(false);
    tempC.setPos(445, 285);
    tempC.setStatus(true);
    vChoice.push_back(tempC);
    tempC.setStatus(false);
    tempC.setPos(445, 335);
    vChoice.push_back(tempC);

    tempC.setVisible(false);
    for (int i = 0; i < 4; ++i) {
        if (i == 3) {
            tempC.setStatus(true);
        }
        tempC.setPos(445, 475 + 50 * i);
        vChoice.push_back(tempC);
    }

    depth = masDepth[3];

    TInput tempI;
    tempI.setThickness(2);
    tempI.setVisible(true);
    tempI.setSize(300, 35);
    tempI.setPos(225, 475);
    tempI.setLimit(20);
    tempI.numbers = true;
    tempI.dot = true;
    tempI.setLimit(15);
    vInput.push_back(tempI);
    tempI.setPos(225, 575);
    tempI.dot = false;
    tempI.setLimit(5);
    vInput.push_back(tempI);


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
void TStartForm::poll() {
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
                        vChoice[0].setStatus(true);
                        vChoice[1].setStatus(false);
                        for (int i = 2; i < 8; ++i) {
                            vChoice[i].setVisible(false);
                        }
                        for (int i = 3; i < 11; ++i) {
                            vLabel[i].setVisible(false);
                        }
                        vInput[0].setVisible(true);
                        vInput[1].setVisible(true);
                        vLabel[11].setVisible(true);
                        vLabel[12].setVisible(true);
                        pvp = true;
                    }
                    else if (index == 1) {
                        vChoice[1].setStatus(true);
                        vChoice[0].setStatus(false);
                        for (int i = 2; i < 8; ++i) {
                            vChoice[i].setVisible(true);
                        }
                        for (int i = 3; i < 11; ++i) {
                            vLabel[i].setVisible(true);
                        }
                        vInput[0].setVisible(false);
                        vInput[1].setVisible(false);
                        vLabel[11].setVisible(false);
                        vLabel[12].setVisible(false);
                        pvp = false;
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
                else if (vInput[0].isPressed(pos)) {
                    vInput[0].onPress();
                    vInput[1].onRelease();
                }
                else if (vInput[1].isPressed(pos)) {
                    vInput[1].onPress();
                    vInput[0].onRelease();
                }
                else if (startB.isPressed(pos)) {
                    if (!pvp/* || pvp && vInput[1].getText().size() && socket.connect(vInput[0].getText(), stoi(vInput[1].getText())) == Socket::Done*/) {
                        win.close();
                        open = true;
                    }
                }
                else if (exitB.isPressed(pos)) {
                    win.close();
                    open = false;
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                char key = event.key.code;
                vInput[0].onKeyPress(key);
                vInput[1].onKeyPress(key);
            }
        }
        draw();
    }
}

void TEngineForm::draw(int posx, int posy) {
    win.clear();
    win.draw(background);
    board.draw(win, posx, posy);
    exitB.draw(win);
    flipB.draw(win);
    analysicsB.draw(win);
    resultLabel.draw(win);
    timeLabel.draw(win);
    win.display();
}
TEngineForm::TEngineForm() : win(VideoMode(1200, 900), "Russian checkers", Style::Close) {

    win.setIcon(512, 512, icon.getPixelsPtr());
    win.setFramerateLimit(150);
    win.setVerticalSyncEnabled(true);

    background.setSize(Vector2f(win.getSize()));
    background.setFillColor(Color::White);

    exitB.setSize(100, 50);
    exitB.setThickness(2);
    exitB.setColor(Color::Green);
    exitB.setText("Exit");
    exitB.setPos(1050, 800 - 2);

    board.setField(control.field);
    board.setPos(100, 50);

    flipB.setSize(100, 50);
    flipB.setColor(Color::Green);
    flipB.setText("Flip");
    flipB.setPos(930, 800 - 2);
    flipB.setThickness(2);

    analysicsB.setSize(200, 60);
    analysicsB.setColor(Color::Green);
    analysicsB.setText("Analysics");
    analysicsB.setPos(950, 100);
    analysicsB.setThickness(2);

    resultLabel.setPos(310, 350);
    resultLabel.setFontSize(100);
    resultLabel.setColor(Color::White);
    resultLabel.setOutlineColor(Color::Black);
    resultLabel.setThickness(7);
    resultLabel.setVisible(false);

    board.setField(control.field);
    engineThread = new Thread(&TEngineForm::engineMove, this);

    timeLabel.setPos(950, 500);
    timeLabel.setText("Time: ");

    if (!turn) {
        board.flip();
        engineThread->launch();
    }
    
}
TEngineForm::~TEngineForm() {
    delete engineThread;
}
void TEngineForm::poll() {

    Vector2f LPPos, LRPos;
    Vector2f pos;
    draw(0, 0);

    while (win.isOpen())
    {
        Event event;
        pos = Vector2f(Mouse::getPosition(win));
        while (win.pollEvent(event))
        {
            if (event.type == Event::Closed) {
                win.close();
                engineThread->wait();
                open = false;
            }
            else if (event.type == Event::MouseButtonPressed) {
                board.capture(pos.x, pos.y);
                if (exitB.isPressed(pos)) {
                    win.close();
                    engineThread->wait();
                    open = true;
                }
                else if (flipB.isPressed(pos)) {
                    board.flip();
                }
                else if (analysicsB.isPressed(pos)) {
                    RenderWindow start(VideoMode(1300, 900), "VOBLA", Style::Close);
                    start.setFramerateLimit(150);
                    start.setVerticalSyncEnabled(true);

                    TAnalysicsForm form1(start, control.gameMoves);
                    form1.poll();
                }
                else {
                    if (event.mouseButton.button == Mouse::Left) {
                        LP = true;
                        LPPos = pos;
                    }
                }
            }
            else if (event.type == Event::MouseButtonReleased) {
                board.uncatch();
                if (LP) {
                    if (event.mouseButton.button == Mouse::Left) {
                        LR = true;
                        LRPos = pos;
                    }
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::Up)) {
                control.getCurr();
                board.setField(control.field);
            }
            else if (Keyboard::isKeyPressed(Keyboard::Left)) {
                control.getPrev();
                board.setField(control.field);
            }
            else if (Keyboard::isKeyPressed(Keyboard::Right)) {
                control.getNext();
                board.setField(control.field);
            }
            if (LP && LR) {
                LP = false;
                LR = false;

                if (turn == control.turn) {
                    mytype coord[4];
                    board.getCoord(LPPos, LRPos, coord);
                    MOVE_RESULT result = control.PlayerMove(coord[0], coord[1], coord[2], coord[3]);
                    if (result != INVALID_COORD) {
                        board.setField(control.field);
                        if (result == SUCCESS) {
                            engineThread->launch();
                        }
                        else if (result == WIN) {
                            resultLabel.setText("You win");
                            resultLabel.setVisible(true);
                        }
                    }
                }
            }
        }
        draw(pos.x, pos.y);
    }
    engineThread->terminate();
}
void TEngineForm::engineMove() {
    auto start = std::chrono::high_resolution_clock::now();

    MOVE_RESULT result;
    do {
        result = control.EngineMove(depth);
        board.setField(control.field);
    } while (result == ONE_MORE);
    if (result == WIN) {
        resultLabel.setText("You lose");
        resultLabel.setVisible(true);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::string s = "Time: " + std::to_string(duration) + " ms\n";
    {
        std::lock_guard<std::mutex> lock(labelMutex);
        timeLabel.setText(s);
    }
}

void TPvpForm::sendMove(mytype x1, mytype y1, mytype x2, mytype y2) {
    if (CheckCoord(x1, y1) && CheckCoord(x2, y2)) {
        Packet packet;
        packet << (uint8_t)MOVEREQ << x1 << y1 << x2 << y2;
        /*socket.send(packet);*/
    }
}
void TPvpForm::resign() {
    Packet packet;
    packet << (uint8_t)RESIGNREQ;
    /*socket.send(packet);*/
}
void TPvpForm::offerdraw() {
    Packet packet;
    packet << (uint8_t)DRAWREQ;
    /*socket.send(packet);*/
}
void TPvpForm::receive() {
    Packet packet;
    uint8_t type;
    int time;
    while (/*socket.receive(packet) == Socket::Done*/true) {
        Vector2f pos = Vector2f(Mouse::getPosition(win));
        packet >> type;
        switch (type) {
        case INIT:
            packet >> turn >> time;
            if (!turn) {
                board.flip();
            }
            wait.setVisible(false);
            clock1.update(time);
            clock2.update(time);
            clock1.start();
            clock2.start();
            if (turn) {
                clock2.pause();
            }
            else {
                clock1.pause();
            }
            break;
        case MOVEREQ:
            mytype x1, y1, x2, y2;
            uint8_t result;
            packet >> result >> x1 >> y1 >> x2 >> y2;
            
            if (result == DRAW) {
                lDraw.setVisible(true);
            }
            else if (result == INVALID_COORD) {
                vMoves.clear();
                board.redReset();
            }
            else if (result == LOSE) {
                control.PlayerMove(x1, y1, x2, y2);
                lLose.setVisible(true);
            }
            else if (result == ONE_MORE || result == SUCCESS) {
                board.redReset();
                control.PlayerMove(x1, y1, x2, y2);
            }
            else if (result == WIN) {
                control.PlayerMove(x1, y1, x2, y2);
                lWin.setVisible(true);
            }

            board.setField(control.field);

            if (control.turn == turn) {
                clock1.release();
                clock2.pause();
            }
            else {
                clock2.release();
                clock1.pause();
            }
            if (result == WIN || result == LOSE || result == DRAW) {
                vMoves.clear();
                board.redReset();
                clock1.stop();
                clock2.stop();
                analysicsB.setVisible(true);
                exitB.setVisible(true);
            }
            break;
        case DRAWREQ:
            drawB.setColor(Color::Red);
            drawB.setText("Accept");
        }
        if (type != DRAWREQ) {
            drawB.setColor(Color::Green);
            drawB.setText("Offer a draw");
        }
        for (const int temp : vMoves) {
            board.redSet(temp / 1000, (temp / 100) % 10, (temp / 10) % 10, temp % 10);
        }
        if (control.turn == turn && vMoves.size()) {
            int temp = vMoves[0];
            sendMove(temp / 1000, (temp / 100) % 10, (temp / 10) % 10, temp % 10);
            vMoves.erase(vMoves.begin());
        }
    }
}
void TPvpForm::draw(int x, int y) {
    win.clear();
    win.draw(background);
    board.draw(win, x, y);
    exitB.draw(win);
    flipB.draw(win);
    analysicsB.draw(win);
    resignB.draw(win);
    drawB.draw(win);
    clock1.draw(win);
    clock2.draw(win);
    wait.draw(win);
    lLose.draw(win);
    lWin.draw(win);
    lDraw.draw(win);
    win.display();
}
void TPvpForm::loading() {
    while (!connected) {
        wait.setNext();
        sleep(milliseconds(300));
    }
}
TPvpForm::TPvpForm() : win(VideoMode(1200, 900), "Russian checkers", Style::Close) {

    win.setIcon(512, 512, icon.getPixelsPtr());
    win.setFramerateLimit(60);
    win.setVerticalSyncEnabled(true);

    background.setSize(Vector2f(win.getSize()));
    background.setFillColor(Color::White);

    exitB.setSize(150, 60);
    exitB.setThickness(2);
    exitB.setColor(Color::Green);
    exitB.setText("Exit");
    exitB.setPos(900, 250);
    exitB.setVisible(false);

    board.setField(control.field);
    board.setPos(50, 50);

    flipB.setSize(120, 50);
    flipB.setColor(Color::Green);
    flipB.setText("Flip");
    flipB.setPos(1030, 700);
    flipB.setThickness(2);

    analysicsB.setSize(150, 60);
    analysicsB.setColor(Color::Green);
    analysicsB.setText("Analysics");
    analysicsB.setPos(900, 180);
    analysicsB.setThickness(2);
    analysicsB.setVisible(false);


    board.setField(control.field);
    wait.setPos(330, 320);
    wait.setRadius(100);


    clock1.setPos(900, 800);
    clock2.setPos(900, 50);

    clock1.update(600);
    clock2.update(600);

    lDraw.setPos(390, 350);
    lDraw.setFontSize(100);
    lDraw.setColor(Color::White);
    lDraw.setOutlineColor(Color::Black);
    lDraw.setThickness(7);
    lDraw.setVisible(false);
    lDraw.setText("Draw");

    lWin.setPos(320, 350);
    lWin.setFontSize(100);
    lWin.setColor(Color::White);
    lWin.setOutlineColor(Color::Black);
    lWin.setThickness(7);
    lWin.setVisible(false);
    lWin.setText("You win");

    lLose.setPos(310, 350);
    lLose.setFontSize(100);
    lLose.setColor(Color::White);
    lLose.setOutlineColor(Color::Black);
    lLose.setThickness(7);
    lLose.setVisible(false);
    lLose.setText("You lose");

    drawB.setPos(900, 640);
    drawB.setSize(180, 50);
    drawB.setText("Offer a draw");
    drawB.setThickness(2);
    drawB.setColor(Color::Green);

    resignB.setPos(900, 700);
    resignB.setSize(120, 50);
    resignB.setText("Resign");
    resignB.setThickness(2);
    resignB.setColor(Color::Green);

    draw(0, 0);
}
void TPvpForm::poll() {

    Vector2f LPPos, LRPos;

    sf::Thread loadingThread(&TPvpForm::loading, this);
    loadingThread.launch();

    sf::Thread receiveTurn(&TPvpForm::receive, this);
    receiveTurn.launch();

    while (win.isOpen())
    {
        Vector2f pos = Vector2f(Mouse::getPosition(win));
        Event event;

        while (win.pollEvent(event))
        {
            if (event.type == Event::Closed) {
                win.close();
                open = false;
            }
            else if (event.type == Event::MouseButtonPressed) {
                board.capture(pos.x, pos.y);
                if (exitB.isPressed(pos)) {
                    win.close();
                    open = true;
                }
                else if (flipB.isPressed(pos)) {
                    board.flip();
                }
                else if (analysicsB.isPressed(pos)) {
                    RenderWindow start(VideoMode(1300, 900), "VOBLA", Style::Close);
                    start.setFramerateLimit(60);
                    start.setVerticalSyncEnabled(true);

                    TAnalysicsForm form1(start, control.gameMoves);
                    form1.poll();
                }
                else if (resignB.isPressed(pos)) {
                    resign();
                }
                else if (drawB.isPressed(pos)) {
                    offerdraw();
                }
                else {
                    if (event.mouseButton.button == Mouse::Left) {
                        LP = true;
                        LPPos = pos;
                    }
                }
            }
            else if (event.type == Event::MouseButtonReleased) {
                board.uncatch();
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
                board.setField(control.field);
            }
            else if (Keyboard::isKeyPressed(Keyboard::Left)) {
                vMoves.clear();
                board.redReset();
                control.getPrev();
                board.setField(control.field);
            }
            else if (Keyboard::isKeyPressed(Keyboard::Right)) {
                control.getNext();
                board.setField(control.field);
            }
            if (LP && LR && control.curr == control.head) {
                LP = false;
                LR = false;

                mytype coord[4];
                board.getCoord(LPPos, LRPos, coord);
                sendMove(coord[0], coord[1], coord[2], coord[3]);
            }
        }
        draw(pos.x, pos.y);
    }

    clock1.stop();
    clock2.stop();
    receiveTurn.terminate();
    loadingThread.terminate();
    /*socket.disconnect();
    listener.close();*/
}