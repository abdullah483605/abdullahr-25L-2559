#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include "SceneManager.h"
#include <SFML/Graphics.hpp>
#include <cmath>

class SplashScreen : public Scene {
private:
    sf::RectangleShape bg;
    sf::CircleShape    decoCircle[5];
    sf::CircleShape    crossBg;
    sf::RectangleShape crossH, crossV;

    sf::Text titleText;
    sf::Text subtitleText;
    sf::Text tagline;
    sf::Text loadingText;

    sf::RectangleShape progressBg;
    sf::RectangleShape progressBar;

    sf::Clock timer;
    float displayTime;

public:
    SplashScreen();
    ~SplashScreen() override;

    void init() override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};

#endif
