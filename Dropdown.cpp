#include "../include/Dropdown.h"

static const float ITEM_H = 36.f;

Dropdown::Dropdown(float px, float py, float pw, float ph, const char* pholder, sf::Font& font)
    : numItems(0), hoveredIdx(-1), selectedIdx(-1), isOpen(false)
{
    x=px; y=py; w=pw; h=ph; fontPtr=&font;
    strCopy(placeholder, pholder);

    mainBox.setPosition(x, y);
    mainBox.setSize(sf::Vector2f(w, h));
    mainBox.setFillColor(sf::Color(255,255,255));
    mainBox.setOutlineThickness(2.f);
    mainBox.setOutlineColor(sf::Color(189,195,199));

    placeholderText.setFont(font);
    placeholderText.setString(placeholder);
    placeholderText.setCharacterSize(16);
    placeholderText.setFillColor(sf::Color(150,150,150));
    placeholderText.setPosition(x+12, y + h/2.f - 10.f);

    selectedText.setFont(font);
    selectedText.setCharacterSize(16);
    selectedText.setFillColor(sf::Color(30,39,46));
    selectedText.setPosition(x+12, y + h/2.f - 10.f);

    listPanel.setFillColor(sf::Color(255,255,255));
    listPanel.setOutlineThickness(1.5f);
    listPanel.setOutlineColor(sf::Color(52,152,219));
}

Dropdown::~Dropdown() {}

void Dropdown::addItem(const char* display, const char* value) {
    if (numItems >= DROPDOWN_MAX) return;
    strCopy(items[numItems], display);
    strCopy(itemValues[numItems], value ? value : display);

    itemBg[numItems].setSize(sf::Vector2f(w, ITEM_H));
    itemBg[numItems].setFillColor(sf::Color(240,244,248));

    itemTxt[numItems].setFont(*fontPtr);
    itemTxt[numItems].setString(display);
    itemTxt[numItems].setCharacterSize(15);
    itemTxt[numItems].setFillColor(sf::Color(44,62,80));

    numItems++;

    // Recompute list panel size + item positions
    listPanel.setPosition(x, y + h);
    listPanel.setSize(sf::Vector2f(w, numItems * ITEM_H));
    for (int i=0; i<numItems; i++) {
        itemBg[i].setPosition(x, y + h + i*ITEM_H);
        itemTxt[i].setPosition(x+12, y + h + i*ITEM_H + ITEM_H/2.f - 9.f);
    }
}

void Dropdown::clearItems() {
    numItems=0; selectedIdx=-1; hoveredIdx=-1; isOpen=false;
    selectedText.setString("");
    mainBox.setOutlineColor(sf::Color(189,195,199));
}

const char* Dropdown::getSelectedText() const {
    if (selectedIdx<0 || selectedIdx>=numItems) return "";
    return items[selectedIdx];
}
const char* Dropdown::getSelectedValue() const {
    if (selectedIdx<0 || selectedIdx>=numItems) return "";
    return itemValues[selectedIdx];
}
void Dropdown::clearSelection() {
    selectedIdx=-1;
    selectedText.setString("");
    isOpen=false;
    mainBox.setOutlineColor(sf::Color(189,195,199));
}

void Dropdown::handleEvent(sf::Event& event, const sf::RenderWindow& window) {
    if (isOpen && event.type == sf::Event::MouseMoved) {
        float mx = static_cast<float>(event.mouseMove.x);
        float my = static_cast<float>(event.mouseMove.y);
        hoveredIdx = -1;
        for (int i=0; i<numItems; i++) {
            sf::FloatRect r = itemBg[i].getGlobalBounds();
            if (mx >= r.left && mx <= r.left+r.width && my >= r.top && my <= r.top+r.height) {
                hoveredIdx = i; break;
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mp(static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y));

        // Click main box
        if (mainBox.getGlobalBounds().contains(mp)) {
            isOpen = !isOpen;
            mainBox.setOutlineColor(isOpen ? sf::Color(52,152,219) : sf::Color(189,195,199));
            return;
        }

        // Click item
        if (isOpen) {
            for (int i=0; i<numItems; i++) {
                if (itemBg[i].getGlobalBounds().contains(mp)) {
                    selectedIdx = i;
                    selectedText.setString(items[i]);
                    isOpen=false;
                    mainBox.setOutlineColor(sf::Color(39,174,96));
                    return;
                }
            }
            isOpen=false;
            mainBox.setOutlineColor(selectedIdx>=0 ? sf::Color(39,174,96) : sf::Color(189,195,199));
        }
    }
}

void Dropdown::update(float dt) {
    // Hover tracking is now processed efficiently within handleEvent
}

void Dropdown::draw(sf::RenderWindow& window) {
    window.draw(mainBox);

    // Arrow indicator
    sf::Text arrow;
    arrow.setFont(*fontPtr);
    arrow.setString(isOpen ? "  ^" : "  v");
    arrow.setCharacterSize(13);
    arrow.setFillColor(sf::Color(100,100,100));
    arrow.setPosition(x + w - 38.f, y + h/2.f - 9.f);
    window.draw(arrow);

    if (selectedIdx < 0)
        window.draw(placeholderText);
    else
        window.draw(selectedText);

    if (isOpen && numItems > 0) {
        window.draw(listPanel);
        for (int i=0; i<numItems; i++) {
            if (i==hoveredIdx)
                itemBg[i].setFillColor(sf::Color(52,152,219,80));
            else if (i==selectedIdx)
                itemBg[i].setFillColor(sf::Color(39,174,96,50));
            else
                itemBg[i].setFillColor((i%2==0) ? sf::Color(245,248,250) : sf::Color(255,255,255));
            window.draw(itemBg[i]);
            window.draw(itemTxt[i]);

            // separator
            sf::RectangleShape sep(sf::Vector2f(w,1));
            sep.setPosition(x, y + h + i*ITEM_H + ITEM_H - 1);
            sep.setFillColor(sf::Color(220,220,220));
            window.draw(sep);
        }
    }
}
