#pragma once
#include <SFML/Network.hpp>
#include "controls.h"
#include <array>
#include <mutex>
#include <vector>
#include <memory>

extern Font font;
extern Image icon;

#define CURRENT_FPS 150

static bool turn = true;
static bool mode = false;
static int depth = 12;
//extern TcpSocket socket;
//extern TcpListener listener;

using std::vector, std::array, std::thread, std::mutex, std::unique_ptr, std::optional, std::string;

class TForm {
public:
    TForm(Vector2u windowSize, const string& title)
        : window(VideoMode(windowSize), title, Style::Close) {

        window.setFramerateLimit(CURRENT_FPS);
        window.setVerticalSyncEnabled(true);

        background.setSize(Vector2f(windowSize));
        background.setFillColor(Color::White);
    }

    virtual ~TForm() { }

    void poll() {

        while (window.isOpen())
        {
            while (const std::optional<Event> event = window.pollEvent()) {
                auto mousePos = Mouse::getPosition(window);

                if (event->is<Event::Closed>()) {
                    onClose();
                }
                else if (event->is<Event::MouseButtonPressed>()) {
                    onLeftButtonPress(mousePos);
                }
                else if (event->is<Event::MouseButtonReleased>()) {
                    onLeftButtonRelease(mousePos);
                }
                else if (event->is<Event::KeyPressed>()) {
                    const auto& keyEvent = event->getIf<Event::KeyPressed>();
                    onKeyDown(keyEvent->code);
                }
                else if (event->is<Event::KeyReleased>()) {
                    // Опционально обработка KeyReleased
                }
                else if (event->is<Event::TextEntered>()) {
                    const auto& textEvent = event->getIf<Event::TextEntered>();
                    char symbol = static_cast<char>(textEvent->unicode);
                    onChar(symbol);
                }
            }
            draw();
        }
    }

    void draw() {
        window.clear();
        window.draw(background);
        onDraw();
        window.display();
    }

protected:
    mutable RenderWindow window;
    RectangleShape background;
    
    virtual void onDraw() const { }

    virtual void onClose() {
        window.close();
    }

    virtual void onKeyDown(Keyboard::Key key) { }

    virtual void onLeftButtonPress(Vector2i position) { }

    virtual void onLeftButtonRelease(Vector2i position) { }

    virtual void onChar(char symbol) { }
};

class TAnalysicsForm final : public TForm {
public:
    TAnalysicsForm(vector<MoveData>& data);

protected:
    void onDraw() const override;

    //void onLeftButtonPress(Vector2i mousePosition) override;

private:
    TButton exitB;
    TBoard board;
    TAssessBar bar;
    TButton flipB;
    TProgressBar pbar;
    AnalysicsController control;
    TCommentSection section;
};

class TEngineForm final: public TForm {
public:
    TEngineForm();
    ~TEngineForm();

protected:
    void onDraw() const override;

private:
    bool LeftPressed = false;
    bool LeftReleased = false;

    TButton exitB, flipB, analysicsB;
    TLabel resultLabel, timeLabel;
    TBoard board;
    GameController control;
    unique_ptr<thread> engineThread;
    mutex labelMutex;

    void engineMove();
};

class TPvpForm final : public TForm {
public:
    TPvpForm();
    ~TPvpForm();

protected:
    void onDraw() const override;


private:
    bool turn{ true };
    bool LP{ false };
    bool LR{ false };
    bool connected{ false };

    vector<int> vMoves;

    TClock clock1, clock2;
    TButton exitB, flipB, analysicsB, drawB, resignB;
    TBoard board;
    TWait wait;
    GameController control;
    TLabel lDraw, lLose, lWin;

    enum Type {
        INIT,
        MOVEREQ,
        DRAWREQ,
        RESIGNREQ
    };

    void resign();
    void offerdraw();
    void sendMove(mytype x1, mytype y1, mytype x2, mytype y2);
    void receive();
    void loading();
};

class TStartForm final : public TForm {
public:
    TStartForm();
    ~TStartForm();

protected:
    void onDraw() const override;

    void onLeftButtonPress(Vector2i mousePosition) override;
    void onChar(char symbol) override;

private:
    vector<TLabel> vLabel;
    vector<TChoice> vChoice;
    vector<TInput> vInput;
    TButton startB, exitB;
    const array<int, 4> masDepth =  { 4, 8, 10, 12 };

    unique_ptr<TEngineForm> engineForm;
    unique_ptr<TPvpForm> pvpForm;
    bool pvp{ false };
    bool turn{ true };
};