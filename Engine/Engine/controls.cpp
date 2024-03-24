#include "controls.h"

const int tileSize = 100;

void TBoard::redReset() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            red[i][j] = false;
        }
    }
}
TBoard::TBoard() {
    x = 0;
    y = 0;
    flipped = false;
}
void TBoard::setField(TField toSet) {
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
void TBoard::setPos(int x0, int y0) {
    x = x0;
    y = y0;
}
void TBoard::getCoord(Vector2f start, Vector2f end, mytype* coord) {
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
    
    coord[0] = x1;
    coord[1] = y1;
    coord[2] = x2;
    coord[3] = y2;

}
void TBoard::redSet(Vector2f& start) {
    int x0 = (start.x - x) / tileSize;
    int y0 = (start.y - y) / tileSize;
    y0 = 7 - y0;

    if ((x0 >= 0) && (y0 >= 0) && (x0 < 8) && (y0 < 8) && ((x0 + y0) % 2 == 0)) {
        red[x0][y0] = !red[x0][y0];
    }
}
void TBoard::flip() {
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
void TBoard::draw(RenderWindow& win) {
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


const int FontSize = 24;
extern Font font;
extern Font font;


TObject::TObject() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    background.setFillColor(Color(230, 230, 230));
    background.setPosition(0, 0);
    background.setSize(Vector2f(0, 0));
    background.setOutlineColor(Color(30, 30, 30));
}
void TObject::setPos(int tx, int ty) {
    x = tx;
    y = ty;
    background.setPosition(Vector2f(x, y));
}
void TObject::setSize(int twidth, int theight) {
    width = twidth;
    height = theight;
    background.setSize(Vector2f(width, height));
}
void TObject::setColor(Color color) {
    background.setFillColor(color);
}
void TObject::setThickness(int thickness) {
    background.setOutlineThickness(thickness);
}
void TObject::draw(RenderWindow& win) {
    win.draw(background);
}


bool TClickable::isPressed(Vector2f& pos) {
    pressPos = pos;
    return ((pos.x - x) >= 0) && (pos.x - x <= width) && ((pos.y - y) >= 0) && (pos.y - y <= height);
}
TClickable::TClickable() : TObject() {}


void TButton::normText() {
    text.setPosition(x + (width - len * FontSize / 2) / 2, y + (height - FontSize) / 2 - 4);
}
TButton::TButton() : TClickable() {
    len = 0;
    text.setPosition(0, 0);
    text.setFont(font);
    text.setCharacterSize(FontSize);
    text.setFillColor(Color::Black);
}
void TButton::setText(std::string toSet) {
    text.setString(toSet);
    len = toSet.size();
    normText();
}
void TButton::setPos(int tx, int ty) {
    TObject::setPos(tx, ty);
    normText();
}
void TButton::setSize(int twidth, int theight) {
    TObject::setSize(twidth, theight);
    normText();
}
void TButton::onPress() {
    background.setFillColor(Color(50, 170, 50));
}
void TButton::onRelease() {
    background.setFillColor(Color::Green);
}
void TButton::draw(RenderWindow& win) {
    TObject::draw(win);
    win.draw(text);
}


TChoice::TChoice() : TClickable() {
    background.setFillColor(Color::White);
    isSelected = false;
    setSize(20, 20);
    setThickness(2);
    in.setFillColor(Color(30, 30, 30));
}
void TChoice::setPos(int tx, int ty) {
    TObject::setPos(tx, ty);
    in.setPosition(tx + width / 4, ty + height / 4);
}
void TChoice::setSize(int twidth, int theight) {
    TObject::setSize(twidth, theight);
    in.setPosition(x + twidth / 4, y + theight / 4);
    in.setSize(Vector2f(twidth / 2, theight / 2));
}
void TChoice::onPress() {
    isSelected = !isSelected;
}
void TChoice::onRelease() { }
void TChoice::draw(RenderWindow& win) {
    TObject::draw(win);
    if (isSelected) {
        win.draw(in);
    }
}
void TChoice::setStatus(bool status) {
    isSelected = status;
}
bool TChoice::getStatus() {
    return isSelected;
}


TBar::TBar() : TObject() {
    value = 0.5;
    posX = width * value;

    first.setPosition(x, y);
    first.setSize(Vector2f(posX, height));
    first.setFillColor(Color::Green);

    second.setPosition(x + posX, y);
    second.setSize(Vector2f(height - posX, height));
    second.setFillColor(Color::White);

    text.setFont(font);
    text.setFillColor(Color::Black);
    text.setPosition(0, 0);
    text.setCharacterSize(FontSize);
    text.setString("");

}
void TBar::setFirstColor(Color color) {
    first.setFillColor(color);
}
void TBar::setSecondColor(Color color) {
    second.setFillColor(color);
}
void TBar::draw(RenderWindow& win) {
    TObject::draw(win);
    win.draw(first);
    win.draw(second);
    win.draw(text);
}


inline void TProgressBar::setWidth() {
    posX = width * value;
}
inline void TProgressBar::setTextPosition() {
    text.setPosition(x + 4, y + (height - FontSize) / 2 - 3);
}
inline void TProgressBar::setString() {
    int toSet = std::round(abs(value) * 100);
    std::string result = std::to_string(toSet);
    result.push_back('%');
    text.setString(result);
}
TProgressBar::TProgressBar() : TBar() { }
void TProgressBar::setPos(int tx, int ty) {
    TObject::setPos(tx, ty);

    first.setPosition(tx, ty);
    second.setPosition(tx + posX, ty);

    setTextPosition();
}
void TProgressBar::setSize(int twidth, int theight) {
    TObject::setSize(twidth, theight);

    first.setSize(Vector2f(twidth, theight));

    second.setSize(Vector2f(width - posX, height));
    second.setPosition(x + posX, y);

    setTextPosition();
}
void TProgressBar::setValue(float toSet) {
    value = toSet;
    setWidth();
    setString();
    setTextPosition();
    first.setSize(Vector2f(posX, height));
    second.setPosition(x + posX, y);
    second.setSize(Vector2f(width - posX, height));
}


inline void TAssessBar::setHeight() {
    if (abs(value) < 0.2) {
        posX = width / 2;
    }
    else if (value > 5) {
        posX = width;
    }
    else if (value < -5) {
        posX = 0;
    }
    else {
        posX = width * (value + 5) / 10;
    }
}
inline void TAssessBar::setTextColor() {
    if (value > 0) {
        text.setFillColor(second.getFillColor());
    }
    else {
        text.setFillColor(first.getFillColor());
    }
}
inline void TAssessBar::setTextPosition() {
    if (value < 0) {
        text.setPosition(x + width - 40, y + (height - FontSize) / 2 - 3);
    }
    else {
        text.setPosition(x + 4, y + (height - FontSize) / 2 - 3);
    }
}
inline void TAssessBar::setString() {
    float toSet = std::round(abs(value) * 100) / 100;
    std::string result = "";
    if (toSet > 0.15) {
        if (toSet > 99) {
            result = "win";
        }
        else {
            result = std::to_string(toSet);
            int dot = result.find(',');
            result = result.substr(0, dot + 2);
            result[dot] = '.';
        }
    }
    text.setString(result);
}
TAssessBar::TAssessBar() : TBar() {
    isFlip = false;
}
void TAssessBar::setPos(int tx, int ty) {
    TObject::setPos(tx, ty);
    first.setPosition(tx, ty);
    second.setPosition(tx, ty + posX);
    setTextPosition();
}
void TAssessBar::setSize(int twidth, int theight) {
    TObject::setSize(twidth, theight);
    first.setSize(Vector2f(twidth, theight));
    second.setSize(Vector2f(width, height - posX));
    second.setPosition(x, y + posX);
    setTextPosition();
}
void TAssessBar::setValue(float toSet) {
    if (isFlip) {
        value = -toSet;
    }
    else {
        value = toSet;
    }

    setHeight();
    setString();
    setTextColor();
    setTextPosition();
    first.setSize(Vector2f(width, posX));
    second.setPosition(x, y + posX);
    second.setSize(Vector2f(width, height - posX));
}
void TAssessBar::flip() {
    isFlip = !isFlip;
    Color temp;
    temp = first.getFillColor();
    first.setFillColor(second.getFillColor());
    second.setFillColor(temp);
}

