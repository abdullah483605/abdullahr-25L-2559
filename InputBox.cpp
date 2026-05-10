#include "../include/InputBox.h"

InputBox::InputBox(float x, float y, float width, float height, const char* ph, sf::Font& font, bool isPass) {
    shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(240, 240, 240));
    shape.setOutlineThickness(2.0f);
    shape.setOutlineColor(sf::Color(200, 200, 200));
    
    capacity = 100;
    content = new char[capacity];
    content[0] = '\0';
    length = 0;
    
    placeholder = allocCopy(ph);
    isPassword = isPass;
    isSelected = false;
    showCursor = false;
    
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x + 10, y + height / 2.0f - 10.0f);
    
    placeholderText.setFont(font);
    placeholderText.setString(placeholder);
    placeholderText.setCharacterSize(18);
    placeholderText.setFillColor(sf::Color(150, 150, 150));
    placeholderText.setPosition(x + 10, y + height / 2.0f - 10.0f);
}

InputBox::~InputBox() {
    delete[] content;
    delete[] placeholder;
}

void InputBox::handleEvent(sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        
        isSelected = shape.getGlobalBounds().contains(mousePosF);
        if (isSelected) {
            shape.setOutlineColor(sf::Color(52, 152, 219)); // Blue border when focused
        } else {
            shape.setOutlineColor(sf::Color(200, 200, 200));
            showCursor = false;
        }
    }
    
    if (isSelected && event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) { // Backspace
            if (length > 0) {
                length--;
                content[length] = '\0';
            }
        } else if (event.text.unicode >= 32 && event.text.unicode < 127 && length < capacity - 1) {
            content[length] = static_cast<char>(event.text.unicode);
            length++;
            content[length] = '\0';
        }
    }
}

void InputBox::update(float dt) {
    if (isSelected) {
        if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
            showCursor = !showCursor;
            cursorClock.restart();
        }
    }
    
    char displayStr[200];
    if (isPassword) {
        for (int i = 0; i < length; i++) displayStr[i] = '*';
        displayStr[length] = '\0';
    } else {
        strCopy(displayStr, content);
    }
    
    if (isSelected && showCursor) {
        strCat(displayStr, "|");
    }
    
    text.setString(displayStr);
}

void InputBox::draw(sf::RenderWindow& window) {
    window.draw(shape);
    if (length == 0 && !isSelected) {
        window.draw(placeholderText);
    } else {
        window.draw(text);
    }
}

const char* InputBox::getText() const {
    return content;
}

void InputBox::setText(const char* t) {
    strCopy(content, t);
    length = strLen(content);
}

void InputBox::clear() {
    length = 0;
    content[0] = '\0';
}

void InputBox::setPosition(float x, float y) {
    shape.setPosition(x, y);
    text.setPosition(x + 10, y + shape.getSize().y / 2.0f - 10.0f);
    placeholderText.setPosition(x + 10, y + shape.getSize().y / 2.0f - 10.0f);
}

void InputBox::setSelected(bool sel) {
    isSelected = sel;
    if (isSelected) {
        shape.setOutlineColor(sf::Color(52, 152, 219));
    } else {
        shape.setOutlineColor(sf::Color(200, 200, 200));
        showCursor = false;
    }
}

bool InputBox::getSelected() const {
    return isSelected;
}
