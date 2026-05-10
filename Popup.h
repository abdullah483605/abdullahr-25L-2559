#ifndef POPUP_H
#define POPUP_H

#include "GUIComponent.h"
#include "Button.h"
#include "Utils.h"

// A simple modal popup for showing messages/confirmations
class Popup : public GUIComponent {
private:
    sf::RectangleShape overlay;    // dark background
    sf::RectangleShape box;
    sf::Text titleText;
    sf::Text messageText;
    Button* okBtn;

    bool visible;
    char* message;
    char* title;

public:
    Popup(sf::Font& font, sf::Vector2u winSize);
    ~Popup();

    void show(const char* t, const char* msg);
    void hide();
    bool isVisible() const { return visible; }
    bool isOkPressed();   // returns true once per click

    void handleEvent(sf::Event& event, const sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};

#endif
