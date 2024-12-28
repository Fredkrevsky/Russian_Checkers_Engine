#include "controls.h"

using std::string;

TBoard::TBoard() : TObject(){

    x1 = x2 = y1 = y2 = 0;
    comment = FORCED;
    flipped = false;

    best.loadFromFile("Image/best.png");
    blunder.loadFromFile("Image/blunder.png");
    forced.loadFromFile("Image/forced.png");
    good.loadFromFile("Image/good.png");
    inac.loadFromFile("Image/inaccurasy.png");
    isCaptured = false;
}

void TBoard::setPos(int x0, int y0) {
    TObject::setPos(x0, y0);
}

void TBoard::setField(TField& toSet) {
    memcpy(field, toSet, 64);
    if (flipped) {
        flip();
        flipped = true;
    }
    isCaptured = false;
}

void TBoard::getCoord(Vector2f start, Vector2f end, mytype coord[]) {

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

void TBoard::flip() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            std::swap(field[i][j], field[7 - i][7 - j]);    //Make std::xxxx();
        }
    }
    flipped = !flipped;
}

void TBoard::draw(RenderWindow& win) const {

    int posx = 0;
    int posy = 0;    //Make class fields

    RectangleShape cell(Vector2f(tileSize, tileSize));
    CircleShape checker(tileSize / 2 - 10);
    CircleShape in(tileSize / 4 - 5);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            if ((i + j) % 2 == 1) {
                cell.setFillColor(Color(233, 237, 204));
            }
            else {
                cell.setFillColor(Color(119, 153, 84));
            }

            cell.setPosition(Vector2f(x + i * tileSize, y + (7 - j) * tileSize));
            win.draw(cell);

            if (field[i][j] != 0) {

                checker.setPosition(Vector2f(x + i * tileSize + 10, y + (7 - j) * tileSize + 10));

                if ((field[i][j] == 1) || (field[i][j] == 3)) {
                    checker.setFillColor(Color(230, 230, 230));
                }
                else if ((field[i][j] == 2) || (field[i][j] == 4)) {
                    checker.setFillColor(Color(30, 30, 30));
                }
                if (!isCaptured || i != cx || j != cy) {
                    win.draw(checker);
                    if (field[i][j] == 3) {
                        in.setPosition(Vector2f(x + i * tileSize + tileSize / 4 + 5, y + (7 - j) * tileSize + tileSize / 4 + 5));
                        in.setFillColor(Color(30, 30, 30));
                        win.draw(in);
                    }
                    else if (field[i][j] == 4) {
                        in.setPosition(Vector2f(x + i * tileSize + tileSize / 4 + 5, y + (7 - j) * tileSize + tileSize / 4 + 5));
                        in.setFillColor(Color(230, 230, 230));
                        win.draw(in);
                    }
                }
            }
        }
    }
    if (x1 || y1 || x2 || y2) {
        Sprite sprite(best);
        switch (comment) {
            //case BEST: sprite.setTexture(best); break;
        case BLUNDER: sprite.setTexture(blunder); break;
        case FORCED: sprite.setTexture(forced); break;
        case INACCURACY: sprite.setTexture(inac); break;
        case GOOD: sprite.setTexture(good); break;
        }
        int tempx, tempy;
        if (!flipped) {
            tempx = x + (x2 + 1) * tileSize - 25;
            tempy = y + (7 - y2) * tileSize - 25;
        }
        else {
            tempx = x + (8 - x2) * tileSize - 25;
            tempy = y + y2 * tileSize - 25;
        }
        sprite.setPosition(Vector2f(tempx, tempy));

        float scaleFactor = 50.0f / sprite.getLocalBounds().size.y;
        sprite.setScale(Vector2f(scaleFactor, scaleFactor));
        win.draw(sprite);
    }
    if (isCaptured) {
        CircleShape checker(tileSize / 2 - 10);
        CircleShape in(tileSize / 4 - 5);

        if ((field[cx][cy] == 1) || (field[cx][cy] == 3)) {
            checker.setFillColor(Color(230, 230, 230));
        }
        else if ((field[cx][cy] == 2) || (field[cx][cy] == 4)) {
            checker.setFillColor(Color(30, 30, 30));
        }
        checker.setPosition(Vector2f(posx - tileSize / 2 + 10, posy - tileSize / 2 + 10));

        if (field[cx][cy]) {
            win.draw(checker);
        }

        if (field[cx][cy] == 3) {
            in.setPosition(Vector2f(posx - tileSize / 4 + 5, posy - tileSize / 4 + 5));
            in.setFillColor(Color(30, 30, 30));
            win.draw(in);
        }
        else if (field[cx][cy] == 4) {
            in.setPosition(Vector2f(posx - tileSize / 4 + 5, posy - tileSize / 4 + 5));
            in.setFillColor(Color(230, 230, 230));
            win.draw(in);
        }
    }
}

void TBoard::capture(int posx, int posy) {
    isCaptured = true;
    cx = (posx - x) / tileSize;
    cy = (posy - y) / tileSize;
    cy = 7 - cy;

    if (cx >= 0 && cx < 8 && cy >= 0 && cy < 8 && (cx + cy) % 2 == 0) {
        isCaptured = true;
    }
}

void TBoard::uncatch() {
    isCaptured = false;
}

void TBoard::setComment(MOVE_STATUS tcomment, mytype tx1, mytype ty1, mytype tx2, mytype ty2) {
    x1 = tx1;
    y1 = ty1;
    x2 = tx2;
    y2 = ty2;
    comment = tcomment;
}

TLabel::TLabel() : text(font) {
    visible = true;
    text.setString("");
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);
    text.setPosition(Vector2f( 0, 0 ));
}

void TLabel::setText(std::string txt) {
    text.setString(txt);
}

void TLabel::setPos(int x, int y) {
    text.setPosition(Vector2f( x, y ));
}

void TLabel::draw(RenderWindow& win) const {
    if (visible) {
        win.draw(text);
    }
}

void TLabel::setVisible(bool toSet) {
    visible = toSet;
}

void TLabel::setThickness(int thick) {
    text.setOutlineThickness(thick);
}

void TLabel::setFontSize(int fontSize) {
    text.setCharacterSize(fontSize);
}

void TLabel::setColor(Color color) {
    text.setFillColor(color);
}

void TLabel::setOutlineColor(Color color) {
    text.setOutlineColor(color);
}


TObject::TObject() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    background.setFillColor(Color(230, 230, 230));
    background.setPosition(Vector2f( 0, 0 ));
    background.setSize(Vector2f(0, 0));
    background.setOutlineColor(Color(30, 30, 30));
    visible = true;
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
void TObject::draw(RenderWindow& win) const {
    if (visible) {
        win.draw(background);
    }
}
void TObject::setVisible(bool _visible) {
    visible = _visible;
}

bool TClickable::isPressed(Vector2f& pos) {
    pressPos = pos;
    return visible && ((pos.x - x) >= 0) && (pos.x - x <= width) && ((pos.y - y) >= 0) && (pos.y - y <= height);
}
TClickable::TClickable() : TObject() {}

void TButton::normText() {
    text.setPosition(Vector2f( x + (width - len * fontSize / 2) / 2 + 4, y + (height - fontSize) / 2 - 4 ));
}
TButton::TButton() : TClickable(), text(font) {
    len = 0;
    text.setPosition(Vector2f( 0, 0 ));
    text.setCharacterSize(fontSize);
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
void TButton::draw(RenderWindow& win) const {
    if (visible) {
        TObject::draw(win);
        win.draw(text);
    }
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
    in.setPosition(Vector2f( tx + width / 4, ty + height / 4 ));
}
void TChoice::setSize(int twidth, int theight) {
    TObject::setSize(twidth, theight);
    in.setPosition(Vector2f( x + twidth / 4, y + theight / 4 ));
    in.setSize(Vector2f(twidth / 2, theight / 2));
}
void TChoice::onPress() {
    isSelected = !isSelected;
}
void TChoice::onRelease() { }
void TChoice::draw(RenderWindow& win) const {
    if (visible) {
        TObject::draw(win);
        if (isSelected) {
            win.draw(in);
        }
    }
}
void TChoice::setStatus(bool status) {
    isSelected = status;
}
bool TChoice::getStatus() {
    return isSelected;
}

TBar::TBar() : TObject(), text(font) {
    value = 0.5;
    posX = width * value;

    first.setPosition(Vector2f( x, y ));
    first.setSize(Vector2f(posX, height));
    first.setFillColor(Color::Green);

    second.setPosition(Vector2f( x + posX, y ));
    second.setSize(Vector2f( height - posX, height ));
    second.setFillColor(Color::White);

    text.setFillColor(Color::Black);
    text.setPosition(Vector2f( 0, 0 ));
    text.setCharacterSize(fontSize);
    text.setString("");

}
void TBar::setFirstColor(Color color) {
    first.setFillColor(color);
}
void TBar::setSecondColor(Color color) {
    second.setFillColor(color);
}
void TBar::draw(RenderWindow& win) const {
    if (visible) {
        TObject::draw(win);
        win.draw(first);
        win.draw(second);
        win.draw(text);
    }
}

inline void TProgressBar::setWidth() {
    posX = width * value;
}
inline void TProgressBar::setTextPosition() {
    text.setPosition(Vector2f( x + 4, y + (height - fontSize) / 2 - 3 ));
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

    first.setPosition(Vector2f( tx, ty ));
    second.setPosition(Vector2f( tx + posX, ty ));

    setTextPosition();
}
void TProgressBar::setSize(int twidth, int theight) {
    TObject::setSize(twidth, theight);

    first.setSize(Vector2f(twidth, theight));

    second.setSize(Vector2f(width - posX, height));
    second.setPosition(Vector2f( x + posX, y ));

    setTextPosition();
}
void TProgressBar::setValue(float toSet) {
    value = toSet;
    setWidth();
    setString();
    setTextPosition();
    first.setSize(Vector2f(posX, height));
    second.setPosition(Vector2f( x + posX, y ));
    second.setSize(Vector2f(width - posX, height));
}

inline void TAssessBar::setHeight() {
    if (abs(value) < 0.2) {
        posX = height / 2;
    }
    else if (value > 5) {
        posX = height;
    }
    else if (value < -5) {
        posX = 0;
    }
    else {
        posX = height * (value + 5) / 10;
    }
    posX = height - posX;
    first.setSize(Vector2f(width, posX));
    second.setPosition(Vector2f( x, y + posX ));
    second.setSize(Vector2f(width, height - posX));
}
inline void TAssessBar::setTextColor() {
    if (value < 0) {
        text.setFillColor(second.getFillColor());
    }
    else {
        text.setFillColor(first.getFillColor());
    }
}
inline void TAssessBar::setTextPosition() {
    if (value < 0) {
        text.setPosition(Vector2f( x + 8, y + 4 ));
    }
    else {
        text.setPosition(Vector2f( x + 8, y + height - fontSize - 4 ));
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
            int dot = result.find('.');
            result = result.substr(0, dot + 2);
        }
    }
    text.setString(result);
}
TAssessBar::TAssessBar() : TBar() {
    isFlip = false;
    text.setCharacterSize(fontSize - 6);
    text.setOutlineThickness(1);
}
void TAssessBar::setPos(int tx, int ty) {
    TObject::setPos(tx, ty);
    first.setPosition(Vector2f( tx, ty ));
    second.setPosition(Vector2f( tx, ty + posX ));
    setTextPosition();
}
void TAssessBar::setSize(int twidth, int theight) {
    TObject::setSize(twidth, theight);
    first.setSize(Vector2f(twidth, theight));
    second.setSize(Vector2f(width, height - posX));
    second.setPosition(Vector2f( x, y + posX ));
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
}
void TAssessBar::flip() {
    isFlip = !isFlip;
    value = -value;
    setHeight();
    setTextPosition();
    Color temp;
    temp = first.getFillColor();
    first.setFillColor(second.getFillColor());
    second.setFillColor(temp);
}

TCommentSection::TCommentSection() : TObject() {
    setThickness(2);
    values.resize(10, 0);

    Text text(font);
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);
    text.setString("Results:");
    text.setPosition(Vector2f( x + 50, y ));
    vText.push_back(text);

    text.setString("0");
    text.setPosition(Vector2f( x, y + 50 ));
    vText.push_back(text);
    text.setString("Best");
    text.setPosition(Vector2f( x + 30, y + 50 ));
    vText.push_back(text);
    text.setString("0");
    text.setPosition(Vector2f( x + 200, y + 50 ));
    vText.push_back(text);

    text.setString("0");
    text.setPosition(Vector2f( x, y + 100 ));
    vText.push_back(text);
    text.setString("Good");
    text.setPosition(Vector2f( x + 30, y + 100 ));
    vText.push_back(text);
    text.setString("0");
    text.setPosition(Vector2f( x + 200, y + 100 ));
    vText.push_back(text);

    text.setString("0");
    text.setPosition(Vector2f( x, y + 150 ));
    vText.push_back(text);
    text.setString("Forced");
    text.setPosition(Vector2f( x + 30, y + 150 ));
    vText.push_back(text);
    text.setString("0");
    text.setPosition(Vector2f( x + 200, y + 150 ));
    vText.push_back(text);

    text.setString("0");
    text.setPosition(Vector2f( x, y + 200 ));
    vText.push_back(text);
    text.setString("Inaccuracy");
    text.setPosition(Vector2f( x + 30, y + 200 ));
    vText.push_back(text);
    text.setString("0");
    text.setPosition(Vector2f( x + 200, y + 200 ));
    vText.push_back(text);

    text.setString("0");
    text.setPosition(Vector2f( x, y + 250 ));
    vText.push_back(text);
    text.setString("Blunder");
    text.setPosition(Vector2f( x + 30, y + 250 ));
    vText.push_back(text);
    text.setString("0");
    text.setPosition(Vector2f( x + 200, y + 250 ));
    vText.push_back(text);

    text.setPosition(Vector2f( x, y + 320 ));
    text.setString("White accuracy:");
    vText.push_back(text);
    text.setPosition(Vector2f( x + 200, y + 320 ));
    text.setString("0.0%");
    vText.push_back(text);

    text.setPosition(Vector2f( x, y + 370 ));
    text.setString("Black accuracy:");
    vText.push_back(text);
    text.setPosition(Vector2f( x + 200, y + 370 ));
    text.setString("0.0%");
    vText.push_back(text);
}
void TCommentSection::setPos(int x0, int y0) {
    TObject::setPos(x0, y0);

    vText[0].setPosition(Vector2f( x0 + 50, y0 ));
    vText[1].setPosition(Vector2f( x0, y0 + 50 ));
    vText[2].setPosition(Vector2f( x0 + 30, y0 + 50 ));
    vText[3].setPosition(Vector2f( x0 + 200, y0 + 50 ));

    vText[4].setPosition(Vector2f( x0, y0 + 100 ));
    vText[5].setPosition(Vector2f( x0 + 30, y0 + 100 ));
    vText[6].setPosition(Vector2f( x0 + 200, y0 + 100 ));

    vText[7].setPosition(Vector2f( x0, y0 + 150 ));
    vText[8].setPosition(Vector2f( x0 + 30, y0 + 150 ));
    vText[9].setPosition(Vector2f( x0 + 200, y0 + 150 ));

    vText[10].setPosition(Vector2f( x0, y0 + 200 ));
    vText[11].setPosition(Vector2f( x0 + 30, y0 + 200 ));
    vText[12].setPosition(Vector2f( x0 + 200, y0 + 200 ));

    vText[13].setPosition(Vector2f( x0, y0 + 250 ));
    vText[14].setPosition(Vector2f( x0 + 30, y0 + 250 ));
    vText[15].setPosition(Vector2f( x0 + 200, y0 + 250 ));

    vText[16].setPosition(Vector2f( x0, y0 + 320 ));
    vText[17].setPosition(Vector2f( x0 + 200, y0 + 320 ));
    vText[18].setPosition(Vector2f( x0, y0 + 370 ));
    vText[19].setPosition(Vector2f( x0 + 200, y0 + 370 ));

}
void TCommentSection::setValues(std::vector<MoveData>& vdata) {
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
        case INACCURACY: 
            if (vdata[i].turn) {
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
void TCommentSection::draw(RenderWindow& win) const {
    TObject::draw(win);
    for (auto& elem : vText) {
        win.draw(elem);
    }
}

void GameController::getData(MoveData& source) {
    assess = source.assess;
    memcpy(field, source.field, 64);
    type = source.type;
    vec = source.vec;
    x = source.x;
    y = source.y;
}
void GameController::setData(MoveData& dest) {
    dest.assess = assess;
    dest.x = x;
    dest.y = y;
    memcpy(dest.field, field, 64);
    memcpy(dest.oldfield, field, 64);
    dest.turn = turn;
    dest.type = type;
    dest.vec = vec;
}
GameController::GameController() {
    type = MOVE;
    x = y = vec = 0;
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
MOVE_RESULT GameController::PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2) {
    if (curr == head && !locked) {

        MoveData data;
        setData(data);
        data.coord[0] = x1;
        data.coord[1] = y1;
        data.coord[2] = x2;
        data.coord[3] = y2;

        MoveData temp = data;
        MOVE_RESULT result = engine.PlayerMove(data);
        if (result != INVALID_COORD) {
            memcpy(temp.field, data.field, 64);
            gameMoves.push_back(temp);
            getData(data);
            if (result == SUCCESS) {
                turn = !turn;
            }
            else if (result == WIN) {
                locked = true;
            }
            head++;
            curr++;
        }
        return result;
    }
}
MOVE_RESULT GameController::EngineMove(mytype depth) {
    if (!locked) {
        getCurr();

        MoveData data;
        setData(data);

        MoveData temp = data;
        MOVE_RESULT result = engine.EngineMove(data, depth);
        if (result == ONE_MORE || result == SUCCESS || result == WIN) {
            memcpy(temp.field, data.field, 64);
            memcpy(temp.coord, data.coord, 4);
            gameMoves.push_back(temp);
            getData(data);
            if (result == SUCCESS) {
                turn = !turn;
            }
            else if (result == WIN) {
                locked = true;
            }
            curr++;
            head++;
        }
        return result;
    }
    return INVALID_COORD;
}
void GameController::getPrev() {
    if (curr > 0) {
        curr--;
        MoveData temp = gameMoves[curr];
        memcpy(field, temp.field, 64);
        assess = temp.assess;
    }
}
void GameController::getNext() {
    if (curr < head) {
        curr++;
        MoveData temp = gameMoves[curr];
        memcpy(field, temp.field, 64);
        assess = temp.assess;
    }
}
void GameController::getCurr() {
    if (curr != head) {
        curr = head;
        MoveData temp = gameMoves[curr];
        memcpy(field, temp.field, 64);
        assess = temp.assess;
    }
}

void AnalysicsController::getData(MoveData& source) {
    assess = source.assess;
    memcpy(field, source.field, 64);
    type = source.type;
    vec = source.vec;
    x = source.x;
    y = source.y;
    x1 = source.coord[0];
    y1 = source.coord[1];
    x2 = source.coord[2];
    y2 = source.coord[3];
    comment = source.comment;
}
AnalysicsController::AnalysicsController() {

    x1 = x2 = y1 = y2 = 0;
    comment = FORCED;
    type = MOVE;
    x = y = vec = 0;
    turn = true;
    curr = 0;
    head = 0;
    assess = 0;
    BInit(field);

}
void AnalysicsController::evaluate(int index, int depth) {
    engine.evaluate(gameMoves[index], depth);
}
void AnalysicsController::setMoves(std::vector<MoveData>& tgameMoves) {
    gameMoves = tgameMoves;
}
void AnalysicsController::getPrev() {
    if (curr > 0) {
        curr--;
        getData(gameMoves[curr]);
    }
}
void AnalysicsController::getNext() {
    if (curr < gameMoves.size() - 1) {
        curr++;
        getData(gameMoves[curr]);
    }
}
void AnalysicsController::getCurr() {
    if (curr != head) {
        curr = head;
        getData(gameMoves[curr]);
    }
}

TInput::TInput() : TClickable(), text(font) {
    text.setPosition(Vector2f( 3, 3 ));
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);
    setColor(Color::White);
    text.setString("");

    limit = 15;
    isSelected = false;
}
bool TInput::checkchar(char toCheck) {
    return (toCheck >= '0') && (toCheck <= '9') || 
        (toCheck >= 'a') && (toCheck <= 'z') || 
        (toCheck >= 'A') && (toCheck <= 'Z') || 
        (toCheck == '_');
}
void TInput::onPress() {
    isSelected = visible;
}
void TInput::onKeyPress(char inputChar) {

    if (isSelected) {
        string temp = text.getString();

        if (letters && inputChar >= 0 && inputChar <= 25) {
            if (temp.size() < limit) {
                temp.push_back('A' + inputChar);
            }
        }
        else if (numbers && inputChar >= 26 && inputChar <= 35) {
            if (temp.size() < limit) {
                temp.push_back('0' + inputChar - 26);
            }
        }
        else if (dot && inputChar == 50) {
            if (temp.size() < limit) {
                temp.push_back('.');
            }
        }
        else if (inputChar == 59 && !temp.empty()) {
            temp.pop_back();
        }
        text.setString(temp);

    }
}
void TInput::onRelease() {
    isSelected = false;
}
void TInput::draw(RenderWindow& win) const {
    if (visible) {
        TClickable::draw(win);
        win.draw(text);
    }
}
void TInput::setPos(int x0, int y0) {
    TClickable::setPos(x0, y0);
    text.setPosition(Vector2f(x0 + 3, y0 + (height - fontSize) / 2 - 4));
}
void TInput::setSize(int w, int h) {
    TClickable::setSize(w, h);
}
void TInput::setLimit(int lim) {
    limit = lim;
}
std::string TInput::getText() {
    return text.getString();
}

TWait::TWait() {
    for (int i = 0; i < 6; ++i) {
        mas[i].setFillColor(Color::White);
        mas[i].setRadius(30);
        mas[i].setOutlineColor(Color::Black);
        mas[i].setOutlineThickness(3);
    }
    setPos();
}
void TWait::setNext() {
    mas[current].setFillColor(Color::White);
    current = (current + 1) % 6;
    mas[current].setFillColor(Color::Green);
}
void TWait::setPos(int tx, int ty) {
    x = tx;
    y = ty;
    setPos();
}
void TWait::setRadius(int tradius) {
    radius = tradius;
    setPos();
}
void TWait::setPos() {
    mas[0].setPosition(Vector2f(x + radius * s3 / 2, y));
    mas[1].setPosition(Vector2f(x + radius * s3, y + radius / 2));
    mas[2].setPosition(Vector2f(x + radius * s3, y + 3 * radius / 2));
    mas[3].setPosition(Vector2f(x + radius * s3 / 2, y + 2 * radius));
    mas[4].setPosition(Vector2f(x, y + 3 * radius / 2));
    mas[5].setPosition(Vector2f(x, y + radius / 2));
}
void TWait::draw(RenderWindow& win) const {
    if (visible) {
        for (int i = 0; i < 6; ++i) {
            win.draw(mas[i]);
        }
    }
}
void TWait::setVisible(bool toSet) {
    visible = toSet;
}


void TClock::tictac() {
    while (gameIsGoing && value > 0) {
        sf::sleep(sf::milliseconds(1000));
        if (yourTurn) {
            value--;
            text.setString(getStringTime(value));
        }
    }
}
std::string TClock::getStringTime(int seconds) {
    return std::to_string(seconds / 60) + ":" + std::to_string(seconds % 60 / 10) + std::to_string(seconds % 10);
}
TClock::TClock() : TObject(), text(font) {
    value = 0;
    setThickness(3);
    background.setFillColor(Color::White);
    text.setString(getStringTime(0));
    text.setFillColor(Color::Black);
    text.setCharacterSize(32);
    //thread = new std::thread(&TClock::tictac, this);
    setSize(150, 50);
}
void TClock::update(int seconds) {
    value = seconds;
    text.setString(getStringTime(seconds));
}
void TClock::setPos(int tx, int ty) {
    TObject::setPos(tx, ty);
    text.setPosition(Vector2f( tx + 47, ty + 4 ));
}
void TClock::start() {
    gameIsGoing = true;
    yourTurn = true;
    //thread->launch();
}
void TClock::pause() {
    background.setFillColor(Color(220, 220, 220));
    yourTurn = false;
}
void TClock::release() {
    background.setFillColor(Color::White);
    yourTurn = true;
}
void TClock::draw(RenderWindow& win) const {
    TObject::draw(win);
    win.draw(text);
}
void TClock::stop() {
    gameIsGoing = false;
    //thread->terminate();
}