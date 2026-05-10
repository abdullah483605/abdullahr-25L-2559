#ifndef GUICOMPONENT_H
#define GUICOMPONENT_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class GUIComponent {
public:
    virtual ~GUIComponent() {}
    virtual void handleEvent(sf::Event& event, const sf::RenderWindow& window) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};

#endif
