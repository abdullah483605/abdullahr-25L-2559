#ifndef INPUTBOX_H
#define INPUTBOX_H

#include "GUIComponent.h"
#include "Utils.h"

class InputBox : public GUIComponent {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Text placeholderText;
    
    char* content;
    int capacity;
    int length;
    
    char* placeholder;
    bool isSelected;
    bool isPassword;
    
    sf::Clock cursorClock;
    bool showCursor;

public:
    InputBox(float x, float y, float width, float height, const char* ph, sf::Font& font, bool isPass = false);
    ~InputBox();

    void handleEvent(sf::Event& event, const sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    const char* getText() const;
    void setText(const char* t);
    void clear();
    void setPosition(float x, float y);
    void setSelected(bool sel);
    bool getSelected() const;
};

#endif
