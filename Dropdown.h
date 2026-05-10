#ifndef DROPDOWN_H
#define DROPDOWN_H

#include "GUIComponent.h"
#include "Utils.h"

#define DROPDOWN_MAX 50

class Dropdown : public GUIComponent {
private:
    sf::RectangleShape mainBox;
    sf::Text           selectedText;
    sf::Text           placeholderText;
    sf::RectangleShape arrowTri[3]; // drawn as a small triangle using 3 thin rects

    // Items storage — no std::vector
    char               items[DROPDOWN_MAX][120];
    char               itemValues[DROPDOWN_MAX][60]; // hidden value (e.g. ID)
    sf::RectangleShape itemBg[DROPDOWN_MAX];
    sf::Text           itemTxt[DROPDOWN_MAX];
    int                numItems;
    int                hoveredIdx;
    int                selectedIdx;

    bool   isOpen;
    float  x, y, w, h;
    char   placeholder[100];
    sf::Font* fontPtr;

    sf::RectangleShape listPanel;

public:
    Dropdown(float x, float y, float w, float h, const char* ph, sf::Font& font);
    ~Dropdown() override;

    // Add item: displayText shown, value is what getSelectedValue() returns
    void addItem(const char* displayText, const char* value = nullptr);
    void clearItems();

    const char* getSelectedText()  const;
    const char* getSelectedValue() const; // returns value (e.g. ID)
    int         getSelectedIndex() const { return selectedIdx; }
    int         itemCount()        const { return numItems; }
    bool        hasSelection()     const { return selectedIdx >= 0; }
    void        clearSelection();
    void        close() { isOpen = false; }

    void handleEvent(sf::Event& event, const sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};

#endif
