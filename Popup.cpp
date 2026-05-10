#include "../include/Popup.h"

Popup::Popup(sf::Font& font, sf::Vector2u winSize) {
    visible = false;
    message = nullptr;
    title   = nullptr;

    // Semi-transparent overlay
    overlay.setSize(sf::Vector2f((float)winSize.x, (float)winSize.y));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));

    // Dialog box
    float bw = 500.f, bh = 220.f;
    float bx = (winSize.x - bw) / 2.f;
    float by = (winSize.y - bh) / 2.f;
    box.setSize(sf::Vector2f(bw, bh));
    box.setPosition(bx, by);
    box.setFillColor(sf::Color(255, 255, 255));
    box.setOutlineThickness(2.f);
    box.setOutlineColor(sf::Color(41, 128, 185));

    titleText.setFont(font);
    titleText.setCharacterSize(22);
    titleText.setFillColor(sf::Color(44, 62, 80));
    titleText.setPosition(bx + 20.f, by + 15.f);

    messageText.setFont(font);
    messageText.setCharacterSize(16);
    messageText.setFillColor(sf::Color(80, 80, 80));
    messageText.setPosition(bx + 20.f, by + 55.f);

    okBtn = new Button(bx + bw / 2.f - 60.f, by + bh - 55.f, 120.f, 38.f, "OK",
                       font,
                       sf::Color(41, 128, 185),
                       sf::Color(31, 97, 141),
                       sf::Color(21, 67, 96));
}

Popup::~Popup() {
    delete okBtn;
    delete[] message;
    delete[] title;
}

void Popup::show(const char* t, const char* msg) {
    delete[] title;   title   = allocCopy(t);
    delete[] message; message = allocCopy(msg);
    titleText.setString(title);
    messageText.setString(message);
    visible = true;
}

void Popup::hide()  { visible = false; }

bool Popup::isOkPressed() {
    if (!visible) return false;
    return okBtn->isPressed();
}

void Popup::handleEvent(sf::Event& event, const sf::RenderWindow& window) {
    if (!visible) return;
    okBtn->handleEvent(event, window);
}

void Popup::update(float dt) {
    if (!visible) return;
    okBtn->update(dt);
    if (okBtn->isPressed()) hide();
}

void Popup::draw(sf::RenderWindow& window) {
    if (!visible) return;
    window.draw(overlay);
    window.draw(box);
    window.draw(titleText);
    window.draw(messageText);
    okBtn->draw(window);
}
