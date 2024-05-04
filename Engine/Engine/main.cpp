#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "forms.h"

bool open = true;
bool turn = true;
bool mode = false;
bool pvp = true;

int depth = 12;

TcpSocket socket;
TcpListener listener;
Font font;
Image icon;

int main()
{
    if (!icon.loadFromFile("Image/icon.png")) {
        return 1;
    }

    if (!font.loadFromFile("Fonts/arialmt.ttf")) {
        return 2;
    }

    while (open) {
        if (open) {
            TStartForm* form = new TStartForm();
            form->poll();
            delete form;
        }
        if (open && !pvp) {
            TEngineForm* form = new TEngineForm();
            form->poll();
            delete form;
        }
        else if (open) {
            TPvpForm* form = new TPvpForm;
            form->poll();
            delete form;
        }
    }

    return 0;
}