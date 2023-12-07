#include "Engine.h"

using namespace sf;

const int menuH = 50;
const int tileSize = 100;

class TChoise {
public:
    RectangleShape field;
    RectangleShape in;
    bool isSelected;
    TChoise(int x0, int y0) {
        field.setSize(Vector2f(20, 20));
        field.setFillColor(Color::White);
        field.setOutlineColor(Color::Black);
        field.setOutlineThickness(2);
        field.setPosition(x0, y0);
        isSelected = false;

        in.setSize(Vector2f(10, 10));
        in.setFillColor(Color::Black);
        in.setPosition(x0 + 5, y0 + 5);
    }
};

void drawCheckerboard(RenderWindow& win, const mytype board[8][8]) {
    for (mytype i = 0; i < 8; i++) {
        for (mytype j = 0; j < 8; j++) {
            RectangleShape cell(Vector2f(tileSize, tileSize));
            if ((i + j) % 2 == 1) {
                cell.setFillColor(Color(238, 238, 213));
            }
            else {
                cell.setFillColor(Color(125, 148, 93));
            }
            cell.setPosition(i * tileSize, (7 - j) * tileSize + menuH);
            win.draw(cell);

            CircleShape checker(tileSize / 2 - 10);
            checker.setPosition(i * tileSize + 10, (7 - j) * tileSize + 10 + menuH);
            if ((board[i][j] == 1) || (board[i][j] == 3)) {
                checker.setFillColor(Color(230, 230, 230));
            }
            else if ((board[i][j] == 2) || (board[i][j] == 4)) {
                checker.setFillColor(Color(30, 30, 30));
            }
            if (board[i][j] != 0) {
                win.draw(checker);
            }
            if (board[i][j] == 3) {
                CircleShape in(tileSize / 4 - 5);
                in.setPosition(i * tileSize + tileSize / 4 + 5, (7 - j) * tileSize + tileSize / 4 + 5 + menuH);
                in.setFillColor(Color(30, 30, 30));
                win.draw(in);
            }
            else if (board[i][j] == 4) {
                CircleShape in(tileSize / 4 - 5);
                in.setPosition(i * tileSize + tileSize / 4 + 5, (7 - j) * tileSize + tileSize / 4 + 5 + menuH);
                in.setFillColor(Color(230, 230, 230));
                win.draw(in);
            }
        }
    }
}

void drawAsses(RenderWindow& window, float asses, Font& font, bool turn) {
    const int x = 800;
    int height = 0;
    asses = std::round(asses * 10) / 10;
    std::string result;
    if ((asses > 9) || (asses < -9)) {
        result = "win";
    }
    else {
        result = std::to_string(asses);
        mytype dot = result.find('.');
        result = result.substr(0, dot + 2);
    }
    Text text(result, font, 15);
    if (asses > 5) {
        height = 800;
    }
    else if (asses < -5) {
        height = 0;
    }
    else {
        height = round(80 * (asses + 5));
    }
    RectangleShape Black(Vector2f(50, 800 - height));
    Black.setFillColor(Color(30, 30, 30));
    RectangleShape White(Vector2f(50, height));
    White.setFillColor(Color(230, 230, 230));
    if (turn) {
        Black.setPosition(x, menuH);
        White.setPosition(x, 800 - height + menuH);
    }
    else {
        Black.setPosition(x, height + menuH);
        White.setPosition(x, 0 + menuH);
    }
    window.draw(Black);
    window.draw(White);
    if (abs(asses) > 0.01) {
        if (turn) {
            if (asses > 0) {
                text.setPosition(x + 16, 770 + menuH);
                text.setFillColor(Color(30, 30, 30));
            }
            else {
                text.setPosition(x + 12, 10 + menuH);
                text.setFillColor(Color(230, 230, 230));
            }
        }
        else {
            if (asses > 0) {
                text.setPosition(x + 16, 10 + menuH);
                text.setFillColor(Color(30, 30, 30));
            }
            else {
                text.setPosition(x + 12, 770 + menuH);
                text.setFillColor(Color(230, 230, 230));
            }
        }
        window.draw(text);
    }
}

void DrawAll(RenderWindow& win, Engine& Vobla, Font& font, bool turn) {
    win.clear();
    RectangleShape menu(Vector2f(850, menuH));
    menu.setFillColor(Color::White);
    menu.setOutlineColor(sf::Color::Black);
    menu.setOutlineThickness(2);
    if (turn) {
        drawCheckerboard(win, Vobla.MainBoard.Field);
    }
    else {
        TField ToPrint;
        for (mytype i = 0; i < 8; i++) {
            for (mytype j = 0; j < 8; j++) {
                ToPrint[i][j] = Vobla.MainBoard.Field[7 - i][7 - j];
            }
        }
        drawCheckerboard(win, ToPrint);
    }
    drawAsses(win, Vobla.asses, font, turn);
    win.draw(menu);
}

void StartScreen(RenderWindow& win, Font& font, bool* turn, mytype* depth) {

    *depth = 5;
    *turn = true;

    bool exit = false;

    RectangleShape fon(Vector2f(850, 850));
    fon.setPosition(0, 0);
    fon.setFillColor(Color(230, 230, 230));


    Text tChoise("Choose your color:", font, 30);
    tChoise.setFillColor(Color::Black);
    tChoise.setPosition(300, 70);

    Text whiteBText("White", font, 30);
    whiteBText.setPosition(245, 130);
    whiteBText.setFillColor(Color::Black);

    Text blackBText("Black", font, 30);
    blackBText.setPosition(245, 180);
    blackBText.setFillColor(Color::Black);

    TChoise choise1(445, 140);
    TChoise choise2(445, 190);


    Text tDiff("Choose the difficulty:", font, 30);
    tDiff.setFillColor(Color::Black);
    tDiff.setPosition(289, 270);

    Text easyBText("Easy", font, 30);
    easyBText.setPosition(245, 330);
    easyBText.setFillColor(Color::Black);

    Text mediumBText("Medium", font, 30);
    mediumBText.setPosition(245, 380);
    mediumBText.setFillColor(Color::Black);

    Text hardBText("Hard", font, 30);
    hardBText.setPosition(245, 430);
    hardBText.setFillColor(Color::Black);

    Text impossibleBText("Impossible", font, 30);
    impossibleBText.setPosition(245, 480);
    impossibleBText.setFillColor(Color::Black);

    TChoise ch1(445, 340);
    TChoise ch2(445, 390);
    TChoise ch3(445, 440);
    TChoise ch4(445, 490);


    Text startBText("Start game", font, 20);
    startBText.setPosition(373, 610);
    startBText.setFillColor(Color::Black);

    RectangleShape startB(sf::Vector2f(200, 50));
    startB.setPosition(325, 600);
    startB.setFillColor(Color::Green);
    startB.setOutlineColor(Color(30, 30, 30));
    startB.setOutlineThickness(2);

    while (win.isOpen() && !exit) {
        Event event;
        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                win.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                Vector2f mousePosition = Vector2f(Mouse::getPosition(win));

                if (choise1.field.getGlobalBounds().contains(mousePosition)) {
                    *turn = true;
                }
                else if (choise2.field.getGlobalBounds().contains(mousePosition)) {
                    *turn = false;
                }
                else if (ch1.field.getGlobalBounds().contains(mousePosition)) {
                    ch1.isSelected = true;
                    ch2.isSelected = false;
                    ch3.isSelected = false;
                    ch4.isSelected = false;
                    *depth = 4;
                }
                else if (ch2.field.getGlobalBounds().contains(mousePosition)) {
                    ch1.isSelected = false;
                    ch2.isSelected = true;
                    ch3.isSelected = false;
                    ch4.isSelected = false;
                    *depth = 5;
                }
                else if (ch3.field.getGlobalBounds().contains(mousePosition)) {
                    ch1.isSelected = false;
                    ch2.isSelected = false;
                    ch3.isSelected = true;
                    ch4.isSelected = false;
                    *depth = 6;
                }
                else if (ch4.field.getGlobalBounds().contains(mousePosition)) {
                    ch1.isSelected = false;
                    ch2.isSelected = false;
                    ch3.isSelected = false;
                    ch4.isSelected = true;
                    *depth = 7;
                }
                else if (startB.getGlobalBounds().contains(mousePosition)) {
                    startBText.setString("Wait a bit...");
                    exit = true;
                }
            }
        }

        win.clear();

        win.draw(fon);
        win.draw(tChoise);

        win.draw(blackBText);
        win.draw(whiteBText);

        win.draw(choise1.field);
        win.draw(choise2.field);
        if (*turn) {
            win.draw(choise1.in);
        }
        else {
            win.draw(choise2.in);
        }
        win.draw(tDiff);
        win.draw(easyBText);
        win.draw(mediumBText);
        win.draw(hardBText);
        win.draw(impossibleBText);
        win.draw(ch1.field);
        win.draw(ch2.field);
        win.draw(ch3.field);
        win.draw(ch4.field);
        if (*depth == 4) {
            win.draw(ch1.in);
        }
        if (*depth == 5) {
            win.draw(ch2.in);
        }
        if (*depth == 6) {
            win.draw(ch3.in);
        }
        if (*depth == 7) {
            win.draw(ch4.in);
        }
        win.draw(startB);
        win.draw(startBText);
        win.display();
    }
}

void MainScreen(RenderWindow& win, Font& font, bool turn, mytype depth) {

    Engine Vobla(depth);

    bool LP = false;
    bool LR = false;


    Text exitBText("Exit to main menu", font, 20);
    exitBText.setPosition(645, 10);
    exitBText.setFillColor(Color::Black);

    RectangleShape exitB(sf::Vector2f(200, 30));
    exitB.setPosition(625, 10);
    exitB.setFillColor(Color::Green);
    exitB.setOutlineColor(Color(30, 30, 30));
    exitB.setOutlineThickness(2);

    Text lose("You lose", font, 150);
    lose.setPosition(120, 250);
    lose.setFillColor(Color::White);
    lose.setOutlineColor(Color(30, 30, 30));
    lose.setOutlineThickness(10);

    Text youwin("You win", font, 150);
    youwin.setPosition(140, 250);
    youwin.setFillColor(Color::White);
    youwin.setOutlineColor(Color(30, 30, 30));
    youwin.setOutlineThickness(10);

    Text isdraw("Draw", font, 150);
    isdraw.setPosition(230, 250);
    isdraw.setFillColor(Color::White);
    isdraw.setOutlineColor(Color(30, 30, 30));
    isdraw.setOutlineThickness(10);

    mytype x1, y1, x2, y2;

    if (!turn) {
        Vobla.EngineMove();
    }
    if (win.isOpen()) {
        DrawAll(win, Vobla, font, turn);
        win.draw(exitB);
        win.draw(exitBText);
        win.display();
    }

    while (win.isOpen())
    {
        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == Event::Closed)
                win.close();

            if (event.type == Event::MouseButtonPressed) {
                Vector2f mousePressPosition = Vector2f(Mouse::getPosition(win));
                if (exitB.getGlobalBounds().contains(mousePressPosition)) {
                    return;
                }
                else {
                    x1 = mousePressPosition.x / tileSize;
                    y1 = (mousePressPosition.y - menuH) / tileSize;
                    if (turn) {
                        y1 = 7 - y1;
                    }
                    else {
                        x1 = 7 - x1;
                    }
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        LP = true;
                    }
                }
            }
            if (event.type == Event::MouseButtonReleased) {
                if (LP) {
                    Vector2i mouseReleasePosition = Mouse::getPosition(win);
                    x2 = mouseReleasePosition.x / tileSize;
                    y2 = (mouseReleasePosition.y - menuH) / tileSize;
                    if (turn) {
                        y2 = 7 - y2;
                    }
                    else {
                        x2 = 7 - x2;
                    }
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        LR = true;
                    }
                }
            }
            if (LP && LR) {
                LP = false;
                LR = false;
                mytype result = Vobla.PlayerMove(x1, y1, x2, y2);
                DrawAll(win, Vobla, font, turn);
                win.draw(exitB);
                win.draw(exitBText);
                if (result == 0) {
                    win.draw(isdraw);
                }
                win.display();
                if (result == 1) {
                    mytype result2 = Vobla.EngineMove();
                    DrawAll(win, Vobla, font, turn);
                    win.draw(exitB);
                    win.draw(exitBText);
                    if (result2 == -1) {
                        win.draw(youwin);
                    }
                    else if (result2 == 1) {
                        win.draw(lose);
                    }
                    win.display();
                }
            }
        }
    }
}

int main()
{

    bool turn = true;
    mytype depth = 5;

    RenderWindow win(VideoMode(850, 850), "VOBLA", Style::Close);

    Image icon;
    if (!icon.loadFromFile("Image/icon.png")) {
        return 1;
    }
    win.setIcon(512, 512, icon.getPixelsPtr());

    Font font;
    if (!font.loadFromFile("Fonts/arialmt.ttf")) {
        return 2;
    }

    while (win.isOpen()) {
        StartScreen(win, font, &turn, &depth);
        if (win.isOpen()) {
            MainScreen(win, font, turn, depth);
        }
    }


    return 0;
}