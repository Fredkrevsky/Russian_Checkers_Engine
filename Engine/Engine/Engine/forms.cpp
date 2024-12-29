#include "forms.h"
#include <mutex>
#include <chrono>

#define PVP_FORM_SIZE Vector2u{1200, 900}
#define START_FORM_SIZE Vector2u{760, 850}
#define ENGINE_FORM_SIZE Vector2u{1200, 900}
#define ANALYSICS_FORM_SIZE Vector2u{1200, 900}

void TAnalysicsForm::onDraw() const {
    bar.draw(window);
    board.draw(window);
    exitB.draw(window);
    flipB.draw(window);
    section.draw(window);
}

TAnalysicsForm::TAnalysicsForm(std::vector<MoveData>& data) :
    TForm(ANALYSICS_FORM_SIZE, "Analysics form") {

    control.setMoves(data);

    board.setField(control.gameMoves[0].field);
    board.setPosition({ 100, 50 });

    bar.setSize({ 46, 8 * tileSize - 4 });
    bar.setPosition({ 100 + 8 * tileSize + 2, 52 });
    bar.setValue(0);
    bar.setThickness(2);

    pbar.setSize({ 600, 50 });
    pbar.setPosition({ 200, 400 });
    pbar.setThickness(2);
    pbar.setValue(0);

    exitB.setSize({ 100, 50 });
    exitB.setThickness(2);
    exitB.setColor(Color::Green);
    exitB.setCaption("Exit");
    exitB.setPosition({ 1130, 798 });

    flipB.setSize({ 100, 50 });
    flipB.setColor(Color::Green);
    flipB.setCaption("Flip");
    flipB.setPosition({ 1010, 798 });
    flipB.setThickness(2);

    section.setPosition({ 1000, 50 });

    bar.setValue(0.0);
    draw();

    for (int i = 1; i < control.gameMoves.size(); i++) {
        control.evaluate(i, 10);
        pbar.setValue((float)i / control.gameMoves.size());
        draw();
    }
    section.setValues(control.gameMoves);
    draw();
}

//void TAnalysicsForm::poll() {
//
//    Vector2f LPPos, LRPos;
//
//    while (win.isOpen())
//    {
//        while (const std::optional event = win.pollEvent())
//        {
//            if (event->is<Event::Closed>()) {
//                onClose();
//                /*win.close();
//                open = false;*/
//            }
//            else if (event->is<Event::MouseButtonPressed>()) {
//                onLeftButtonPress();
//                /*Vector2f pos = Vector2f(Mouse::getPosition(win));
//                if (exitB.isPressed(pos)) {
//                    win.close();
//                    open = true;
//                }
//                else if (flipB.isPressed(pos)) {
//                    board.flip();
//                    bar.flip();
//                }*/
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
//                onKeyDown();
//                /*control.getCurr();
//                board.setField(control.field);
//                board.setComment(control.comment, control.x1, control.y1, control.x2, control.y2);
//                bar.setValue(control.assess);*/
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
//                /*control.getPrev();
//                board.setField(control.field);
//                board.setComment(control.comment, control.x1, control.y1, control.x2, control.y2);
//                bar.setValue(control.assess);*/
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
//                /*control.getNext();
//                board.setField(control.field);
//                board.setComment(control.comment, control.x1, control.y1, control.x2, control.y2);
//                bar.setValue(control.assess);*/
//            }
//        }
//        draw();
//    }
//}

void TStartForm::onDraw() const {
    std::ranges::for_each(vLabel, [&](const auto& label) {
        label.draw(window);
    });
    std::ranges::for_each(vChoice, [&](const auto& choice) {
        choice.draw(window);
    });
    std::ranges::for_each(vInput, [&](const auto& input) {
        input.draw(window);
    });
    startB.draw(window);
    exitB.draw(window);
}

TStartForm::TStartForm() : TForm(START_FORM_SIZE, "Start form") {

    background.setSize(Vector2f(window.getSize()));
    background.setFillColor(Color::White);

    vLabel.push_back(TLabel("Choose mode:", Vector2f(320, 30)));
    vLabel.push_back(TLabel("vs Player", Vector2f(245, 90)));
    vLabel.push_back(TLabel("vs Engine", Vector2f(245, 140)));

    vLabel.push_back(TLabel("Choose your color:", Vector2f(300, 220), false));
    vLabel.push_back(TLabel("White", Vector2f(245, 280), false));
    vLabel.push_back(TLabel("Black", Vector2f(245, 330), false));
    vLabel.push_back(TLabel("Choose the difficulty:", Vector2f(289, 410), false));
    vLabel.push_back(TLabel("Easy", Vector2f(245, 470), false));
    vLabel.push_back(TLabel("Medium", Vector2f(245, 520), false));
    vLabel.push_back(TLabel("Hard", Vector2f(245, 570), false));
    vLabel.push_back(TLabel("Impossible", Vector2f(245, 620), false));

    vLabel.push_back(TLabel("Enter server IP", Vector2f(300, 425)));
    vLabel.push_back(TLabel("Enter server port", Vector2f(300, 525)));

    turn = true;

    TChoice tempC;
    tempC.setPosition({ 445, 97 });
    tempC.setStatus(true);
    vChoice.push_back(tempC);

    tempC.setPosition({ 445, 147 });
    tempC.setStatus(false);
    vChoice.push_back(tempC);

    tempC.setVisible(false);
    tempC.setPosition({ 445, 285 });
    tempC.setStatus(true);
    vChoice.push_back(tempC);
    tempC.setStatus(false);
    tempC.setPosition({ 445, 335 });
    vChoice.push_back(tempC);

    tempC.setVisible(false);
    for (int i = 0; i < 4; ++i) {
        if (i == 3) {
            tempC.setStatus(true);
        }
        const float newY = 475 + 50.f * i;
        tempC.setPosition({ 445, newY});
        vChoice.push_back(tempC);
    }

    depth = masDepth[3];

    TInput tempI;
    tempI.setThickness(2);
    tempI.setVisible(true);
    tempI.setSize({ 300, 35 });
    tempI.setPosition({ 225, 475 });
    tempI.setLimit(20);
    tempI.numbers = true;
    tempI.dot = true;
    tempI.setLimit(15);
    vInput.push_back(tempI);
    tempI.setPosition({ 225, 575 });
    tempI.dot = false;
    tempI.setLimit(5);
    vInput.push_back(tempI);


    startB.setSize({ 125, 50 });
    startB.setColor(Color::Green);
    startB.setCaption("Start");
    startB.setPosition({ 240, 720 });
    startB.setThickness(2);

    exitB.setSize({ 125, 50 });
    exitB.setColor(Color::Green);
    exitB.setCaption("Exit");
    exitB.setPosition({ 400, 720 });
    exitB.setThickness(2);

    draw();
}

TStartForm::~TStartForm() { }

//void TStartForm::poll() {
//
//    while (win.isOpen()) {
//
//        while (const std::optional event = win.pollEvent()) {
//            if (event->is<Event::Closed>()) {
//                onClose();
//                /*win.close();
//                open = false;*/
//            }
//            else if (event->is<Event::MouseButtonPressed>()) {
//                onLeftButtonPress();
//                //Vector2f pos = Vector2f(Mouse::getPosition(win));
//
//                //int index = -1;
//                //for (int i = 0; i < 8; i++) {
//                //    if (vChoice[i].isPressed(pos)) {
//                //        index = i;
//                //        break;
//                //    }
//                //}
//                //if (index != -1) {
//                //    if (index == 0) {
//                //        vChoice[0].setStatus(true);
//                //        vChoice[1].setStatus(false);
//                //        for (int i = 2; i < 8; ++i) {
//                //            vChoice[i].setVisible(false);
//                //        }
//                //        for (int i = 3; i < 11; ++i) {
//                //            vLabel[i].setVisible(false);
//                //        }
//                //        vInput[0].setVisible(true);
//                //        vInput[1].setVisible(true);
//                //        vLabel[11].setVisible(true);
//                //        vLabel[12].setVisible(true);
//                //        pvp = true;
//                //    }
//                //    else if (index == 1) {
//                //        vChoice[1].setStatus(true);
//                //        vChoice[0].setStatus(false);
//                //        for (int i = 2; i < 8; ++i) {
//                //            vChoice[i].setVisible(true);
//                //        }
//                //        for (int i = 3; i < 11; ++i) {
//                //            vLabel[i].setVisible(true);
//                //        }
//                //        vInput[0].setVisible(false);
//                //        vInput[1].setVisible(false);
//                //        vLabel[11].setVisible(false);
//                //        vLabel[12].setVisible(false);
//                //        pvp = false;
//                //    }
//                //    else if (index == 2) {
//                //        turn = true;
//                //        vChoice[2].setStatus(true);
//                //        vChoice[3].setStatus(false);
//                //    }
//                //    else if (index == 3) {
//                //        turn = false;
//                //        vChoice[2].setStatus(false);
//                //        vChoice[3].setStatus(true);
//                //    }
//                //    else {
//                //        for (int i = 4; i < 8; i++) {
//                //            if (i != index) {
//                //                vChoice[i].setStatus(false);
//                //            }
//                //            else {
//                //                vChoice[i].setStatus(true);
//                //            }
//                //        }
//                //        depth = masDepth[index - 4];
//                //    }
//                //}
//                //else if (vInput[0].isPressed(pos)) {
//                //    vInput[0].onPress();
//                //    vInput[1].onRelease();
//                //}
//                //else if (vInput[1].isPressed(pos)) {
//                //    vInput[1].onPress();
//                //    vInput[0].onRelease();
//                //}
//                //else if (startB.isPressed(pos)) {
//                //    if (!pvp/* || pvp && vInput[1].getText().size() && socket.connect(vInput[0].getText(), stoi(vInput[1].getText())) == Socket::Done*/) {
//                //        win.close();
//                //        open = true;
//                //    }
//                //}
//                //else if (exitB.isPressed(pos)) {
//                //    win.close();
//                //    open = false;
//                //}
//            }
//            else if (event->is<Event::TextEntered>()) {
//                onChar();
//                //char key = 'a';//static_cast<char>(event.value.text.scancode);
//                //vInput[0].onKeyPress(key);
//                //vInput[1].onKeyPress(key);
//            }
//        }
//        draw();
//    }
//}

void TEngineForm::onDraw() const {
    board.draw(window);
    exitB.draw(window);
    flipB.draw(window);
    analysicsB.draw(window);
    resultLabel.draw(window);
    timeLabel.draw(window);
}

TEngineForm::TEngineForm() : TForm(ENGINE_FORM_SIZE, "Engine Form") {

    exitB.setSize({ 100, 50 });
    exitB.setThickness(2);
    exitB.setColor(Color::Green);
    exitB.setCaption("Exit");
    exitB.setPosition({ 1050, 798 });

    board.setField(control.field);
    board.setPosition({ 100, 50 });

    flipB.setSize({ 100, 50 });
    flipB.setColor(Color::Green);
    flipB.setCaption("Flip");
    flipB.setPosition({ 930, 798 });
    flipB.setThickness(2);

    analysicsB.setSize({ 200, 60 });
    analysicsB.setColor(Color::Green);
    analysicsB.setCaption("Analysics");
    analysicsB.setPosition({ 950, 100 });
    analysicsB.setThickness(2);

    resultLabel.setPosition({ 310, 350 });
    resultLabel.setFontSize(100);
    resultLabel.setColor(Color::White);
    resultLabel.setOutlineColor(Color::Black);
    resultLabel.setThickness(7);
    resultLabel.setVisible(false);

    timeLabel.setPosition({ 950, 500 });
    timeLabel.setText("Time: ");

    board.setField(control.field);
    //engineThread = new Thread(&TEngineForm::engineMove, this);

    if (!turn) {
        board.flip();
        //engineThread->launch();
    }
    
}

TEngineForm::~TEngineForm() { }

//void TEngineForm::poll() {
//
//    Vector2f LPPos, LRPos;
//    Vector2f pos;
//    draw();
//
//    while (win.isOpen())
//    {
//        pos = Vector2f(Mouse::getPosition(win));
//        while (const std::optional event = win.pollEvent()) {
//            if (event->is<Event::Closed>()) {
//                win.close();
//                //engineThread->wait();
//                open = false;
//            }
//            else if (event->is<Event::MouseButtonPressed>()) {
//                board.capture(pos.x, pos.y);
//                if (exitB.isPressed(pos)) {
//                    win.close();
//                    //engineThread->wait();
//                    open = true;
//                }
//                else if (flipB.isPressed(pos)) {
//                    board.flip();
//                }
//                else if (analysicsB.isPressed(pos)) {
//                    std::unique_ptr<RenderWindow> analysicsWindow{new RenderWindow(VideoMode({ 1300, 900 }), "VOBLA", Style::Close)};
//                    analysicsWindow->setFramerateLimit(150);
//                    analysicsWindow->setVerticalSyncEnabled(true);
//
//                    std::unique_ptr<TAnalysicsForm> analysicsForm{ new TAnalysicsForm(*analysicsWindow.get(), control.gameMoves)};
//                    analysicsForm->poll();
//                }
//                else if (Mouse::isButtonPressed(Mouse::Button::Left)) {
//                    LP = true;
//                    LPPos = pos;
//                }
//            }
//            else if (event->is<Event::MouseButtonReleased>()) {
//                board.uncatch();
//                if (LP && Mouse::isButtonPressed(Mouse::Button::Left)) {
//                    LR = true;
//                    LRPos = pos;
//                }
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
//                control.getCurr();
//                board.setField(control.field);
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
//                control.getPrev();
//                board.setField(control.field);
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
//                control.getNext();
//                board.setField(control.field);
//            }
//            if (LP && LR) {
//                LP = false;
//                LR = false;
//
//                if (turn == control.turn) {
//                    mytype coord[4];
//                    board.getCoord(LPPos, LRPos, coord);
//                    MOVE_RESULT result = control.PlayerMove(coord[0], coord[1], coord[2], coord[3]);
//                    if (result != INVALID_COORD) {
//                        board.setField(control.field);
//                        if (result == SUCCESS) {
//                            //engineThread->launch();
//                        }
//                        else if (result == WIN) {
//                            resultLabel.setText("You win");
//                            resultLabel.setVisible(true);
//                        }
//                    }
//                }
//            }
//        }
//        board.setTilePos({pos.x, pos.y});
//        draw(pos.x, pos.y);
//    }
//    //engineThread->terminate();
//}

TPvpForm::TPvpForm() : TForm(PVP_FORM_SIZE, "PvP form") {

    exitB.setSize({ 150, 60 });
    exitB.setThickness(2);
    exitB.setColor(Color::Green);
    exitB.setCaption("Exit");
    exitB.setPosition({ 900, 250 });
    exitB.setVisible(false);

    board.setField(control.field);
    board.setPosition({ 50, 50 });

    flipB.setSize({ 120, 50 });
    flipB.setColor(Color::Green);
    flipB.setCaption("Flip");
    flipB.setPosition({ 1030, 700 });
    flipB.setThickness(2);

    analysicsB.setSize({ 150, 60 });
    analysicsB.setColor(Color::Green);
    analysicsB.setCaption("Analysics");
    analysicsB.setPosition({ 900, 180 });
    analysicsB.setThickness(2);
    analysicsB.setVisible(false);


    board.setField(control.field);
    wait.setPosition({ 330, 320 });
    wait.setRadius(100);


    clock1.setPosition({ 900, 800 });
    clock2.setPosition({ 900, 50 });

    clock1.update(600);
    clock2.update(600);

    lDraw.setPosition({ 390, 350 });
    lDraw.setFontSize(100);
    lDraw.setColor(Color::White);
    lDraw.setOutlineColor(Color::Black);
    lDraw.setThickness(7);
    lDraw.setVisible(false);
    lDraw.setText("Draw");

    lWin.setPosition({ 320, 350 });
    lWin.setFontSize(100);
    lWin.setColor(Color::White);
    lWin.setOutlineColor(Color::Black);
    lWin.setThickness(7);
    lWin.setVisible(false);
    lWin.setText("You win");

    lLose.setPosition({ 310, 350 });
    lLose.setFontSize(100);
    lLose.setColor(Color::White);
    lLose.setOutlineColor(Color::Black);
    lLose.setThickness(7);
    lLose.setVisible(false);
    lLose.setText("You lose");

    drawB.setPosition({ 900, 640 });
    drawB.setSize({ 180, 50 });
    drawB.setCaption("Offer a draw");
    drawB.setThickness(2);
    drawB.setColor(Color::Green);

    resignB.setPosition({ 900, 700 });
    resignB.setSize({ 120, 50 });
    resignB.setCaption("Resign");
    resignB.setThickness(2);
    resignB.setColor(Color::Green);

    draw();
}

TPvpForm::~TPvpForm() {

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
        Vector2f pos = Vector2f(Mouse::getPosition(window));
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
            }
            else if (result == LOSE) {
                control.PlayerMove(x1, y1, x2, y2);
                lLose.setVisible(true);
            }
            else if (result == ONE_MORE || result == SUCCESS) {
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
                clock1.stop();
                clock2.stop();
                analysicsB.setVisible(true);
                exitB.setVisible(true);
            }
            break;
        case DRAWREQ:
            drawB.setColor(Color::Red);
            drawB.setCaption("Accept");
        }
        if (type != DRAWREQ) {
            drawB.setColor(Color::Green);
            drawB.setCaption("Offer a draw");
        }
        if (control.turn == turn && vMoves.size()) {
            int temp = vMoves[0];
            sendMove(temp / 1000, (temp / 100) % 10, (temp / 10) % 10, temp % 10);
            vMoves.erase(vMoves.begin());
        }
    }
}

void TPvpForm::onDraw() const {
    board.draw(window);
    exitB.draw(window);
    flipB.draw(window);
    analysicsB.draw(window);
    resignB.draw(window);
    drawB.draw(window);
    clock1.draw(window);
    clock2.draw(window);
    wait.draw(window);
    lLose.draw(window);
    lWin.draw(window);
    lDraw.draw(window);
}

void TPvpForm::loading() {
    while (!connected) {
        wait.setNext();
        sleep(milliseconds(300));   // condition variable
    }
}

//void TPvpForm::poll() {
//
//    Vector2f LPPos, LRPos;
//
//    std::thread loadingThread(&TPvpForm::loading, this);
//
//    std::thread receiveTurn(&TPvpForm::receive, this);
//
//    while (win.isOpen())
//    {
//        Vector2f pos = Vector2f(Mouse::getPosition(win));
//
//        while (const std::optional event = win.pollEvent())
//        {
//            if (event->is<Event::Closed>()) {
//                win.close();
//                open = false;
//            }
//            else if (event->is<Event::MouseButtonPressed>()) {
//                board.capture(pos.x, pos.y);
//                if (exitB.isPressed(pos)) {
//                    win.close();
//                    open = true;
//                }
//                else if (flipB.isPressed(pos)) {
//                    board.flip();
//                }
//                else if (analysicsB.isPressed(pos)) {
//                    TAnalysicsForm form1(control.gameMoves);
//                    form1.poll();
//                }
//                else if (resignB.isPressed(pos)) {
//                    resign();
//                }
//                else if (drawB.isPressed(pos)) {
//                    offerdraw();
//                }
//                else {
//                    if (Mouse::isButtonPressed(Mouse::Button::Left)) {
//                        LP = true;
//                        LPPos = pos;
//                    }
//                }
//            }
//            else if (event->is<Event::MouseButtonReleased>()) {
//                board.uncatch();
//                if (LP) {
//                    Vector2f pos = Vector2f(Mouse::getPosition(win));
//                    if (Mouse::isButtonPressed(Mouse::Button::Left)) {
//                        LR = true;
//                        LRPos = pos;
//                    }
//                }
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
//                control.getCurr();
//                board.setField(control.field);
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
//                vMoves.clear();
//                control.getPrev();
//                board.setField(control.field);
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
//                control.getNext();
//                board.setField(control.field);
//            }
//            if (LP && LR && control.curr == control.head) {
//                LP = false;
//                LR = false;
//
//                mytype coord[4];
//                board.getCoord(LPPos, LRPos, coord);
//                sendMove(coord[0], coord[1], coord[2], coord[3]);
//            }
//        }
//        draw();
//    }
//
//    clock1.stop();
//    clock2.stop();
//    //receiveTurn.terminate();
//    //loadingThread.terminate();
//    /*socket.disconnect();
//    listener.close();*/
//}