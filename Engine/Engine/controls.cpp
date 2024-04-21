#include "controls.h"

void TBoard::redReset() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            red[i][j] = false;
        }
    }
}
TBoard::TBoard() : TObject(){

    x1 = x2 = y1 = y2 = 0;
    comment = FORCED;
    flipped = false;

    best.loadFromFile("Image/best.png");
    blunder.loadFromFile("Image/blunder.png");
    forced.loadFromFile("Image/forced.png");
    good.loadFromFile("Image/good.png");
    inac.loadFromFile("Image/inaccurasy.png");
}
void TBoard::setPos(int x0, int y0) {
    TObject::setPos(x0, y0);
}
void TBoard::setField(TField& toSet) {
    BCopy(field, toSet);
    if (flipped) {
        flip();
        flipped = true;
    }
}
void TBoard::getCoord(Vector2f start, Vector2f end, mytype* coord) {

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
void TBoard::redSet(mytype x1, mytype y1, mytype x2, mytype y2) {
    if (flipped) {
        x1 = 7 - x1;
        x2 = 7 - x2;
        y1 = 7 - y1;
        y2 = 7 - y2;
    }

    if ((x1 >= 0) && (y1 >= 0) && (x1 < 8) && (y1 < 8) && ((x1 + y1) % 2 == 0)) {
        red[x1][y1] = true;
    }
    if ((x2 >= 0) && (y2 >= 0) && (x2 < 8) && (y2 < 8) && ((x2 + y2) % 2 == 0)) {
        red[x2][y2] = true;
    }
}
void TBoard::redReset(mytype x1, mytype y1, mytype x2, mytype y2) {
    if (flipped) {
        x1 = 7 - x1;
        x2 = 7 - x2;
        y1 = 7 - y1;
        y2 = 7 - y2;
    }

    if ((x1 >= 0) && (y1 >= 0) && (x1 < 8) && (y1 < 8) && ((x1 + y1) % 2 == 0)) {
        red[x1][y1] = false;
    }
    if ((x2 >= 0) && (y2 >= 0) && (x2 < 8) && (y2 < 8) && ((x2 + y2) % 2 == 0)) {
        red[x2][y2] = false;
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
    RectangleShape cell(Vector2f(tileSize, tileSize));
    CircleShape checker(tileSize / 2 - 10);
    CircleShape in(tileSize / 4 - 5);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

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

                checker.setPosition(x + i * tileSize + 10, y + (7 - j) * tileSize + 10);

                if ((field[i][j] == 1) || (field[i][j] == 3)) {
                    checker.setFillColor(Color(230, 230, 230));
                }
                else if ((field[i][j] == 2) || (field[i][j] == 4)) {
                    checker.setFillColor(Color(30, 30, 30));
                }
                win.draw(checker);
                if (field[i][j] == 3) {
                    in.setPosition(x + i * tileSize + tileSize / 4 + 5, y + (7 - j) * tileSize + tileSize / 4 + 5);
                    in.setFillColor(Color(30, 30, 30));
                    win.draw(in);
                }
                else if (field[i][j] == 4) {
                    in.setPosition(x + i * tileSize + tileSize / 4 + 5, y + (7 - j) * tileSize + tileSize / 4 + 5);
                    in.setFillColor(Color(230, 230, 230));
                    win.draw(in);
                }
            }
        }
    }
    if (x1 || y1 || x2 || y2) {
        Sprite sprite;
        switch (comment) {
            case BEST: sprite.setTexture(best); break;
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
        sprite.setPosition(tempx, tempy);

        float scaleFactor = 50.0f / sprite.getLocalBounds().width;
        sprite.setScale(scaleFactor, scaleFactor);
        win.draw(sprite);
    }
}
void TBoard::setComment(MOVE_STATUS tcomment, mytype tx1, mytype ty1, mytype tx2, mytype ty2) {
    x1 = tx1;
    y1 = ty1;
    x2 = tx2;
    y2 = ty2;
    comment = tcomment;
}

TLabel::TLabel() {
    visible = true;
    text.setString("");
    text.setFont(font);
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);
    text.setPosition(0, 0);
}
void TLabel::setText(std::string txt) {
    text.setString(txt);
}
void TLabel::setPos(int x, int y) {
    text.setPosition(x, y);
}
void TLabel::draw(RenderWindow& win) {
    if (visible) {
        win.draw(text);
    }
}
void TLabel::setVisible(bool toSet) {
    visible = toSet;
}

TObject::TObject() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    background.setFillColor(Color(230, 230, 230));
    background.setPosition(0, 0);
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
void TObject::draw(RenderWindow& win) {
    if (visible) {
        win.draw(background);
    }
}
void TObject::setVisible(bool toSet) {
    visible = toSet;
}

bool TClickable::isPressed(Vector2f& pos) {
    pressPos = pos;
    return visible && ((pos.x - x) >= 0) && (pos.x - x <= width) && ((pos.y - y) >= 0) && (pos.y - y <= height);
}
TClickable::TClickable() : TObject() {}

void TButton::normText() {
    text.setPosition(x + (width - len * fontSize / 2) / 2 + 4, y + (height - fontSize) / 2 - 4);
}
TButton::TButton() : TClickable() {
    len = 0;
    text.setPosition(0, 0);
    text.setFont(font);
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
void TButton::draw(RenderWindow& win) {
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
    text.setCharacterSize(fontSize);
    text.setString("");

}
void TBar::setFirstColor(Color color) {
    first.setFillColor(color);
}
void TBar::setSecondColor(Color color) {
    second.setFillColor(color);
}
void TBar::draw(RenderWindow& win) {
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
    text.setPosition(x + 4, y + (height - fontSize) / 2 - 3);
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
    second.setPosition(x, y + posX);
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
        text.setPosition(x + 8, y + 4);
    }
    else {
        text.setPosition(x + 8, y + height - fontSize - 4);
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
void TCommentSection::setPos(int x0, int y0) {
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
void TCommentSection::draw(RenderWindow& win) {
    TObject::draw(win);
    for (auto& elem : vText) {
        win.draw(elem);
    }
}

void GameController::getData(MoveData& source) {
    assess = source.assess;
    BCopy(field, source.field);
    type = source.type;
    vector = source.vector;
    x = source.x;
    y = source.y;
}
void GameController::setData(MoveData& dest) {
    dest.assess = assess;
    dest.x = x;
    dest.y = y;
    BCopy(dest.field, field);
    BCopy(dest.oldfield, field);
    dest.turn = turn;
    dest.type = type;
    dest.vector = vector;
}
GameController::GameController() {
    type = MOVE;
    x = y = vector = 0;
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
    if (curr == head) {

        MoveData data;
        setData(data);
        data.coord[0] = x1;
        data.coord[1] = y1;
        data.coord[2] = x2;
        data.coord[3] = y2;

        MoveData temp = data;
        MOVE_RESULT result = engine.PlayerMove(data);
        if (result != INVALID_COORD) {
            BCopy(temp.field, data.field);
            gameMoves.push_back(temp);
            getData(data);
            if (result == SUCCESS) {
                turn = !turn;
            }
            head++;
            curr++;
        }
        return result;
    }
}
MOVE_RESULT GameController::EngineMove(mytype depth) {
    getCurr();

    MoveData data;
    setData(data);

    MoveData temp = data;
    MOVE_RESULT result = engine.EngineMove(data, depth);
    if (result == ONE_MORE || result == SUCCESS) {
        BCopy(temp.field, data.field);
        temp.coord[0] = data.coord[0];
        temp.coord[1] = data.coord[1];
        temp.coord[2] = data.coord[2];
        temp.coord[3] = data.coord[3];
        gameMoves.push_back(temp);
        getData(data);
        if (result == SUCCESS) {
            turn = !turn;
        }
        curr++;
        head++;
    }
    return result;
}
void GameController::getPrev() {
    if (curr > 0) {
        curr--;
        MoveData temp = gameMoves[curr];
        BCopy(field, temp.field);
        assess = temp.assess;
    }
}
void GameController::getNext() {
    if (curr < head) {
        curr++;
        MoveData temp = gameMoves[curr];
        BCopy(field, temp.field);
        assess = temp.assess;
    }
}
void GameController::getCurr() {
    if (curr != head) {
        curr = head;
        MoveData temp = gameMoves[curr];
        BCopy(field, temp.field);
        assess = temp.assess;
    }
}

void AnalysicsController::getData(MoveData& source) {
    assess = source.assess;
    BCopy(field, source.field);
    type = source.type;
    vector = source.vector;
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
    x = y = vector = 0;
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

TInput::TInput() : TClickable() {
    text.setPosition(3, 3);
    text.setCharacterSize(fontSize);
    text.setFont(font);
    text.setFillColor(Color::Black);
    setColor(Color::White);
    text.setString("");

    limit = 15;
    isSelected = false;
}
bool TInput::checkchar(char toCheck) {
    return (toCheck >= '0') && (toCheck <= '9') || (toCheck >= 'a') && (toCheck <= 'z') || (toCheck >= 'A') && (toCheck <= 'Z') || (toCheck == '_');
}
void TInput::onPress() {
    isSelected = visible;
}
void TInput::onKeyPress(char inputChar) {

    if (isSelected) {
        std::string temp = text.getString();

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
void TInput::draw(RenderWindow& win) {
    if (visible) {
        TClickable::draw(win);
        win.draw(text);
    }
}
void TInput::setPos(int x0, int y0) {
    TClickable::setPos(x0, y0);
    text.setPosition(x0 + 3, y0 + (height - fontSize) / 2 - 4);
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
    mas[0].setPosition(x + radius * s3 / 2, y);
    mas[1].setPosition(x + radius * s3, y + radius / 2);
    mas[2].setPosition(x + radius * s3, y + 3 * radius / 2);
    mas[3].setPosition(x + radius * s3 / 2, y + 2 * radius);
    mas[4].setPosition(x, y + 3 * radius / 2);
    mas[5].setPosition(x, y + radius / 2);
}
void TWait::draw(RenderWindow& win) {
    if (visible) {
        for (int i = 0; i < 6; ++i) {
            win.draw(mas[i]);
        }
    }
}
void TWait::setVisible(bool toSet) {
    visible = toSet;
}