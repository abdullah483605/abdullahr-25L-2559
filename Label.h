#ifndef LABEL_H
#define LABEL_H

#include "GUIComponent.h"
#include "Utils.h"

class Label : public GUIComponent {
private:
    sf::Text text;
    char* content;

public:
    Label(float x, float y, const char* t, sf::Font& font, int size, sf::Color color);
    ~Label();

    void handleEvent(sf::Event& event, const sf::RenderWindow& window) override {}
    void update(float dt) override {}
    void draw(sf::RenderWindow& window) override;

    void setText(const char* t);
    void setPosition(float x, float y);
    void setColor(sf::Color color);
};

#endif
