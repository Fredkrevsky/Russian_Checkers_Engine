#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "forms.h"

//TcpSocket socket;
//TcpListener listener;
sf::Font font;
sf::Image icon;

using std::unique_ptr;

int main()
{
    if (!icon.loadFromFile("Image/icon.png")) {
        return 1;
    }

    if (!font.openFromFile("Fonts/arialmt.ttf")) {
        return 2;
    }

    unique_ptr<TForm> mainForm{new TStartForm()};
    mainForm->poll();

    return 0;
}