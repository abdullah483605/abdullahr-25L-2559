#include "../include/Button.h"

Button::Button(float x, float y, float width, float height, const char* t, sf::Font& font, sf::Color idle, sf::Color hover, sf::Color active) {
    idleColor = idle;
    hoverColor = hover;
    activeColor = active;
    
    shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(idleColor);
    shape.setOutlineThickness(2.0f);
    shape.setOutlineColor(sf::Color(0, 0, 0, 50));
    
    btnText = allocCopy(t);
    text.setFont(font);
    text.setString(btnText);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(20);
    
    // Center text
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(x + width / 2.0f, y + height / 2.0f);
    
    isHovered = false;
    isClicked = false;
    actionTriggered = false;
}

Button::~Button() {
    delete[] btnText;
}

void Button::handleEvent(sf::Event& event, const sf::RenderWindow& window) {
    actionTriggered = false;
    
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    
    isHovered = shape.getGlobalBounds().contains(mousePosF);
    
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (isHovered) {
            isClicked = true;
        }
    }
    
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (isHovered && isClicked) {
            actionTriggered = true;
        }
        isClicked = false;
    }
}

void Button::update(float dt) {
    if (isClicked) {
        shape.setFillColor(activeColor);
    } else if (isHovered) {
        shape.setFillColor(hoverColor);
    } else {
        shape.setFillColor(idleColor);
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::isPressed() {
    if (actionTriggered) {
        actionTriggered = false;
        return true;
    }
    return false;
}

void Button::setText(const char* t) {
    delete[] btnText;
    btnText = allocCopy(t);
    text.setString(btnText);
    
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f, shape.getPosition().y + shape.getSize().y / 2.0f);
}

void Button::setTextColor(sf::Color color) {
    text.setFillColor(color);
}

void Button::setPosition(float x, float y) {
    shape.setPosition(x, y);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setPosition(x + shape.getSize().x / 2.0f, y + shape.getSize().y / 2.0f);
}
