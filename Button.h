#ifndef BUTTON_H
#define BUTTON_H

#include "GUIComponent.h"
#include "Utils.h"

class Button : public GUIComponent {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;
    
    char* btnText;
    bool isHovered;
    bool isClicked;
    bool actionTriggered;

public:
    Button(float x, float y, float width, float height, const char* t, sf::Font& font, sf::Color idle, sf::Color hover, sf::Color active);
    ~Button();

    void handleEvent(sf::Event& event, const sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    bool isPressed();
    void setText(const char* t);
    void setTextColor(sf::Color color);
    void setPosition(float x, float y);
};

#endif
