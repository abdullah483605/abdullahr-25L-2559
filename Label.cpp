#include "../include/Label.h"

Label::Label(float x, float y, const char* t, sf::Font& font, int size, sf::Color color) {
    content = allocCopy(t);
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(x, y);
}

Label::~Label() {
    delete[] content;
}

void Label::draw(sf::RenderWindow& window) {
    window.draw(text);
}

void Label::setText(const char* t) {
    delete[] content;
    content = allocCopy(t);
    text.setString(content);
}

void Label::setPosition(float x, float y) {
    text.setPosition(x, y);
}

void Label::setColor(sf::Color color) {
    text.setFillColor(color);
}
