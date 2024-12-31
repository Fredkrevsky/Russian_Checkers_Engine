#include "controls.h"
#include <algorithm>

using std::string, std::to_string, std::round, std::swap;


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

void TBoard::setPosition(Vector2f position) {
    TObject::setPosition(position);
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

    const auto& [x, y] = position;

    mytype x1 = static_cast<mytype>((start.x - x) / tileSize);
    mytype y1 = static_cast<mytype>(7 - (start.y - y) / tileSize);

    mytype x2 = static_cast<mytype>((end.x - x) / tileSize);
    mytype y2 = static_cast<mytype>(7 - (end.y - y) / tileSize);

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
    std::reverse(field, field + 8);
    std::for_each(field, field + 8, [](auto row[]) {
        std::reverse(row, row + 8);
    });
    flipped = !flipped;
}

void TBoard::draw(RenderWindow& window) const {

    float posx = 0;
    float posy = 0;    //Make class fields

    RectangleShape cell(Vector2f(tileSize, tileSize));
    CircleShape checkerCircle(tileSize / 2 - 10);
    CircleShape innerCircle(tileSize / 4 - 5);

    Color whiteColor(230, 230, 230), blackColor(30, 30, 30);
    Color boardFirstColor(233, 237, 204), boardSecondColor(119, 153, 84);

    const auto& [x, y] = position;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {

            const auto piece{ field[i][j] };

            if ((i + j) % 2 == 1) {
                cell.setFillColor(boardFirstColor);
            }
            else {
                cell.setFillColor(boardSecondColor);
            }

            cell.setPosition(Vector2f(x + i * tileSize, y + (7 - j) * tileSize));
            window.draw(cell);

            if (piece != 0) {

                checkerCircle.setPosition(Vector2f(x + i * tileSize + 10, y + (7 - j) * tileSize + 10));

                if (piece == 1 || piece == 3) {
                    checkerCircle.setFillColor(whiteColor);
                }
                else if (piece == 2 || piece == 4) {
                    checkerCircle.setFillColor(blackColor);
                }
                if (!isCaptured || i != cx || j != cy) {
                    window.draw(checkerCircle);
                    if (piece == 3) {
                        innerCircle.setPosition(Vector2f(x + i * tileSize + tileSize / 4 + 5, y + (7 - j) * tileSize + tileSize / 4 + 5));
                        innerCircle.setFillColor(blackColor);
                        window.draw(innerCircle);
                    }
                    else if (piece == 4) {
                        innerCircle.setPosition(Vector2f(x + i * tileSize + tileSize / 4 + 5, y + (7 - j) * tileSize + tileSize / 4 + 5));
                        innerCircle.setFillColor(whiteColor);
                        window.draw(innerCircle);
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
        float tempx, tempy;
        if (!flipped) {
            tempx = x + (x2 + 1) * tileSize - 25;
            tempy = y + (7 - y2) * tileSize - 25;
        }
        else {
            tempx = x + (8 - x2) * tileSize - 25;
            tempy = y + y2 * tileSize - 25;
        }
        sprite.setPosition({ tempx, tempy });

        const float scaleFactor = 50.0f / sprite.getLocalBounds().size.y;
        sprite.setScale({ scaleFactor, scaleFactor });
        window.draw(sprite);
    }
    if (isCaptured) {
        CircleShape checkerCircle(tileSize / 2 - 10);
        CircleShape innerCircle(tileSize / 4 - 5);

        if ((field[cx][cy] == 1) || (field[cx][cy] == 3)) {
            checkerCircle.setFillColor(Color(230, 230, 230));
        }
        else if ((field[cx][cy] == 2) || (field[cx][cy] == 4)) {
            checkerCircle.setFillColor(Color(30, 30, 30));
        }
        checkerCircle.setPosition(Vector2f(posx - tileSize / 2 + 10, posy - tileSize / 2 + 10));

        if (field[cx][cy]) {
            window.draw(checkerCircle);
        }

        if (field[cx][cy] == 3) {
            innerCircle.setPosition(Vector2f(posx - tileSize / 4 + 5, posy - tileSize / 4 + 5));
            innerCircle.setFillColor(Color(30, 30, 30));
            window.draw(innerCircle);
        }
        else if (field[cx][cy] == 4) {
            innerCircle.setPosition(Vector2f(posx - tileSize / 4 + 5, posy - tileSize / 4 + 5));
            innerCircle.setFillColor(Color(230, 230, 230));
            window.draw(innerCircle);
        }
    }
}

void TBoard::capture(int posx, int posy) {
    const auto& [x, y] = position;

    cx = static_cast<mytype>((posx - x) / tileSize);
    cy = static_cast<mytype>(7 - (posy - y) / tileSize);

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
    text.setPosition({ 0, 0 });
}

TLabel::TLabel(const string& _caption, const Vector2f _position, bool _visible) : text(font) {
    TObject::setPosition(_position);
    visible = _visible;

    text.setString(_caption);
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);
    text.setPosition(_position);
}

void TLabel::setText(const string& _text) {
    text.setString(_text);
}

void TLabel::setPosition(Vector2f position) {
    TObject::setPosition(position);
    text.setPosition(position);
}

void TLabel::draw(RenderWindow& window) const {
    if (visible) {
        window.draw(text);
    }
}

void TLabel::setTextThickness(float thick) {
    text.setOutlineThickness(thick);
}

void TLabel::setFontSize(int fontSize) {
    text.setCharacterSize(fontSize);
}

void TLabel::setTextColor(Color color) {
    text.setFillColor(color);
}

void TLabel::setOutlineColor(Color color) {
    text.setOutlineColor(color);
}


TObject::TObject() {
    background.setFillColor(Color::White);
    background.setPosition(Vector2f( 0, 0 ));
    background.setSize(Vector2f(0, 0));
    background.setOutlineColor(Color(30, 30, 30));
    visible = true;
}

void TObject::setPosition(Vector2f _position) {
    position = _position;
    background.setPosition(position);
}

void TObject::setSize(Vector2f _size) {
    size = _size;
    background.setSize(size);
}

void TObject::setColor(Color color) {
    background.setFillColor(color);
}

void TObject::setThickness(float thickness) {
    background.setOutlineThickness(thickness);
}

void TObject::draw(RenderWindow& window) const {
    if (visible) {
        window.draw(background);
    }
}

void TObject::setVisible(bool _visible) {
    visible = _visible;
}


bool TClickable::isPressed(Vector2i mousePosition) const {
    auto& [posx, posy] = mousePosition;
    auto& [x, y] = position;
    auto& [width, height] = size;

    return visible && 
        ((posx - x) >= 0) && 
        (posx - x <= width) && 
        ((posy - y) >= 0) && 
        (posy - y <= height);
}

void TClickable::setOnPress(const function<void()>& callback) {
    onPressFunction = callback;
}

TClickable::TClickable() : TObject() {}

void TClickable::onPress() {
    if (onPressFunction) {
        onPressFunction();
    }
}

void TButton::normCaption() {
    const auto& captionString = caption.getString();
    const int captionLength = static_cast<int>(captionString.getSize());

    const auto& [x, y] = position;
    const auto& [width, height] = size;

    const float newX = x + (width - captionLength * fontSize / 2) / 2 + 4;
    const float newY = y + (height - fontSize) / 2 - 4;
    caption.setPosition({ newX, newY });
}

TButton::TButton() : caption(font) {
    caption.setPosition({0, 0});
    caption.setCharacterSize(fontSize);
    caption.setFillColor(Color::Black);

    setColor(Color::Green);
    setThickness(2);
    setSize({ 125, 50 });
}

TButton::TButton(const string& _caption, Vector2f _position) : caption(font) {
    caption.setCharacterSize(fontSize);
    caption.setFillColor(Color::Black);
    caption.setString(_caption);

    setColor(Color::Green);
    setThickness(2);

    setPosition(_position);
    setSize({ 125, 50 });
}

void TButton::setCaption(const string& _caption) {
    caption.setString(_caption);
    normCaption();
}

void TButton::setPosition(Vector2f _position) {
    TObject::setPosition(_position);
    normCaption();
}

void TButton::setSize(Vector2f _size) {
    TObject::setSize(_size);
    normCaption();
}

void TButton::onPress() {
    //background.setFillColor(Color(50, 170, 50));
    TClickable::onPress();
}

void TButton::onRelease() {
    background.setFillColor(Color::Green);
}

void TButton::draw(RenderWindow& window) const {
    if (visible) {
        TObject::draw(window);
        window.draw(caption);
    }
}


TChoice::TChoice() {
    status = false;
    setSize({ 20, 20 });
    setThickness(2);
    in.setFillColor(Color(30, 30, 30));
}

TChoice::TChoice(Vector2f _position, const function<void()>& callback, bool _status, bool _visible) {
    status = _status;
    visible = _visible;
    onPressFunction = callback;
    setPosition(_position);
    setSize({20, 20});
    setThickness(2);
    in.setFillColor(Color(30, 30, 30));
}

void TChoice::setPosition(Vector2f _position) {
    TObject::setPosition(_position);

    const auto& [x, y] = position;
    const auto& [width, height] = size;

    const float newX = x + width / 4.f;
    const float newY = y + height / 4.f;
    in.setPosition({newX, newY});
}

void TChoice::setSize(Vector2f _size) {
    TObject::setSize(_size);

    const auto& [x, y] = position;
    const auto& [width, height] = size;

    const float newX = x + width / 4.f;
    const float newY = y + height / 4.f;
    const float newWidth = width / 2.f;
    const float newHeight = height / 2.f;

    in.setPosition({newX, newY});
    in.setSize({ newWidth, newHeight });
}

void TChoice::onRelease() { }

void TChoice::draw(RenderWindow& window) const {
    if (visible) {
        TObject::draw(window);
        if (status) {
            window.draw(in);
        }
    }
}

void TChoice::setStatus(bool _status) {
    status = _status;
}

bool TChoice::getStatus() {
    return status;
}


TBar::TBar() : text(font) {
    const auto& [x, y] = position;
    const auto& [width, height] = size;

    value = 0.5f;
    posX = width * value;

    first.setPosition(position);
    first.setSize({ posX, height });
    first.setFillColor(Color::Green);

    second.setPosition({ x + posX, y });
    second.setSize({ height - posX, height });
    second.setFillColor(Color::White);

    text.setFillColor(Color::Black);
    text.setPosition({0, 0});
    text.setCharacterSize(fontSize);
}

void TBar::setFirstColor(Color color) {
    first.setFillColor(color);
}

void TBar::setSecondColor(Color color) {
    second.setFillColor(color);
}

void TBar::draw(RenderWindow& window) const {
    if (visible) {
        TObject::draw(window);
        window.draw(first);
        window.draw(second);
        window.draw(text);
    }
}

inline void TProgressBar::setWidth() {
    const float width = size.x;
    posX = width * value;
}

inline void TProgressBar::setTextPosition() {
    const auto& [x, y] = position;
    const auto& [width, height] = size;

    const float newX = x + 4;
    const float newY = y + (height - fontSize) / 2.f - 3;
    text.setPosition({ newX, newY });
}

inline void TProgressBar::setString() {
    const int toSet = static_cast<int>(round(abs(value) * 100));
    string result = to_string(toSet) + "%";
    text.setString(result);
}

TProgressBar::TProgressBar() : TBar() { }

void TProgressBar::setPosition(Vector2f _position) {
    TObject::setPosition(_position);
    first.setPosition(_position);

    const auto& [x, y] = _position;
    second.setPosition({ x + posX, y });
    setTextPosition();
}

void TProgressBar::setSize(Vector2f _size) {
    TObject::setSize(_size);
    first.setSize(_size);

    const auto& [x, y] = position;
    const auto& [width, height] = _size;
    second.setSize({ width - posX, height });
    second.setPosition({ x + posX, y });

    setTextPosition();
}

void TProgressBar::setValue(float _value) {
    value = _value;
    setWidth();
    setString();
    setTextPosition();

    const auto& [x, y] = position;
    const auto& [width, height] = size;

    first.setSize({ posX, height });
    second.setPosition({ x + posX, y });
    second.setSize({ width - posX, height });
}


inline void TAssessBar::setHeight() {

    const auto& [x, y] = position;
    const auto& [width, height] = size;

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
    first.setSize({ width, posX });
    second.setPosition({ x, y + posX });
    second.setSize({ width, height - posX });
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
    const auto& [x, y] = position;
    const auto& [width, height] = size;

    const float newX = x + 8;
    const float newY = value < 0
        ? y + 4
        : y + height - fontSize - 4;

    text.setPosition({ newX, newY });
}

inline void TAssessBar::setString() {
    float toSet = round(abs(value) * 100) / 100;
    string result = "";
    if (toSet > 0.15) {
        if (toSet > 99) {
            result = "win";
        }
        else {
            result = to_string(toSet);
            const int dot = static_cast<int>(result.find('.'));
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

void TAssessBar::setPosition(Vector2f _position) {
    TObject::setPosition(_position);
    first.setPosition(_position);

    const auto& [x, y] = _position;
    second.setPosition({ x, y + posX });
    setTextPosition();
}

void TAssessBar::setSize(Vector2f _size) {
    TObject::setSize(_size);
    first.setSize(_size);

    const auto& [x, y] = position;
    const auto& [width, height] = _size;
    second.setSize({ width, height - posX });
    second.setPosition({ x, y + posX });
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

    const auto& [x, y] = position;

    //TODO: CLEAN THIS SHIT
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

void TCommentSection::setPosition(Vector2f _position) {
    TObject::setPosition(_position);

    const auto& [x, y] = _position;

    vText[0].setPosition(Vector2f( x + 50, y ));
    vText[1].setPosition(Vector2f( x, y + 50 ));
    vText[2].setPosition(Vector2f( x + 30, y + 50 ));
    vText[3].setPosition(Vector2f( x + 200, y + 50 ));

    vText[4].setPosition(Vector2f( x, y + 100 ));
    vText[5].setPosition(Vector2f( x + 30, y + 100 ));
    vText[6].setPosition(Vector2f( x + 200, y + 100 ));

    vText[7].setPosition(Vector2f( x, y + 150 ));
    vText[8].setPosition(Vector2f( x + 30, y + 150 ));
    vText[9].setPosition(Vector2f( x + 200, y + 150 ));

    vText[10].setPosition(Vector2f( x, y + 200 ));
    vText[11].setPosition(Vector2f( x + 30, y + 200 ));
    vText[12].setPosition(Vector2f( x + 200, y + 200 ));

    vText[13].setPosition(Vector2f( x, y + 250 ));
    vText[14].setPosition(Vector2f( x + 30, y + 250 ));
    vText[15].setPosition(Vector2f( x + 200, y + 250 ));

    vText[16].setPosition(Vector2f( x, y + 320 ));
    vText[17].setPosition(Vector2f( x + 200, y + 320 ));
    vText[18].setPosition(Vector2f( x, y + 370 ));
    vText[19].setPosition(Vector2f( x + 200, y + 370 ));

}

void TCommentSection::setValues(vector<MoveData>& vdata) {
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
        vText[3 * i + 1].setString(to_string(values[2 * i]));
        vText[3 * i + 3].setString(to_string(values[2 * i + 1]));
    }
    if (sumwhite) {
        vText[17].setString(to_string(white * 100 / sumwhite) + "." + to_string((white * 1000 / sumwhite) % 10) + "%");
    }
    if (sumblack) {
        vText[19].setString(to_string(black * 100 / sumblack) + "." + to_string((black * 1000 / sumblack) % 10) + "%");
    }
}

void TCommentSection::draw(RenderWindow& window) const {
    TObject::draw(window);
    std::ranges::for_each(vText, [&window](const auto& elem) {
        window.draw(elem);
    });
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
    return MOVE_RESULT::INVALID_COORD;
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

void AnalysicsController::setMoves(vector<MoveData>& tgameMoves) {
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


TInput::TInput() : text(font) {
    text.setPosition(Vector2f( 3, 3 ));
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);
    setColor(Color::White);
    text.setString("");

    isSelected = false;
}

TInput::TInput(Vector2f _position, MODE mode, int _maxTextLength) : text(font) {
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);

    maxTextLength = _maxTextLength;

    switch (mode) {
    case IP:
        numbers = true;
        dot = true;
        break;
    case PORT: 
        numbers = true;
        break;
    }
}

void TInput::onPress() {
    isSelected = visible;
}

void TInput::onKeyPress(char symbol) {

    if (isSelected && visible) {
        string currentString = text.getString();
        const int size = static_cast<int>(currentString.size());

        if (size < maxTextLength) {
            if (letters && std::isalpha(symbol)) {
                currentString.push_back(symbol);
            }
            else if (numbers && std::isdigit(symbol)) {
                currentString.push_back(symbol);
            }
            else if (dot && symbol == '.') {
                currentString.push_back(symbol);
            }
        }
        if (symbol == '\b' && !currentString.empty()) {
            currentString.pop_back();
        }
        text.setString(currentString);
    }
}

void TInput::onRelease() {
    isSelected = false;
}

void TInput::draw(RenderWindow& window) const {
    if (visible) {
        TClickable::draw(window);
        window.draw(text);
    }
}

void TInput::setPosition(Vector2f _position) {
    TClickable::setPosition(_position);

    const auto& [x, y] = _position;
    const auto& [width, height] = size;

    const float newX = x + 3;
    const float newY = y + (height - fontSize) / 2.f - 4;
    text.setPosition({newX, newY});
}

void TInput::setSize(Vector2f _size) {
    TClickable::setSize(_size);
}

void TInput::setLimit(int _maxTextLength) {
    maxTextLength = _maxTextLength;
}

string TInput::getText() {
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

void TWait::setPosition(Vector2f _position) {
    TObject::setPosition(_position);
}

void TWait::setRadius(int tradius) {
    radius = tradius;
    setPos();
}

void TWait::setPos() {
    const auto& [x, y] = position;

    mas[0].setPosition({ x + radius * s3 / 2, y });
    mas[1].setPosition({ x + radius * s3, y + radius / 2 });
    mas[2].setPosition({ x + radius * s3, y + 3 * radius / 2 });
    mas[3].setPosition({ x + radius * s3 / 2, y + 2 * radius });
    mas[4].setPosition({ x, y + 3 * radius / 2 });
    mas[5].setPosition({ x, y + radius / 2 });
}

void TWait::draw(RenderWindow& window) const {
    if (visible) {
        std::ranges::for_each(mas, [&window](const auto& elem) {
            window.draw(elem);
        });
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

string TClock::getStringTime(int seconds) {
    return to_string(seconds / 60) + ":" + to_string(seconds % 60 / 10) + to_string(seconds % 10);
}

TClock::TClock() : TObject(), text(font) {
    value = 0;
    setThickness(3);
    background.setFillColor(Color::White);
    text.setString(getStringTime(0));
    text.setFillColor(Color::Black);
    text.setCharacterSize(32);
    //thread = new std::thread(&TClock::tictac, this);
    setSize({150, 50});
}

void TClock::update(int seconds) {
    value = seconds;
    text.setString(getStringTime(seconds));
}

void TClock::setPosition(Vector2f _position) {
    TObject::setPosition(position);

    const auto& [x, y] = _position;
    text.setPosition({ x + 47, y + 4 });
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

void TClock::draw(RenderWindow& window) const {
    TObject::draw(window);
    window.draw(text);
}

void TClock::stop() {
    gameIsGoing = false;
    //thread->terminate();
}