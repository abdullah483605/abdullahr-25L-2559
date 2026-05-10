#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class SceneManager; // Forward declaration

class Scene {
public:
    virtual ~Scene() {}
    virtual void init() = 0;
    virtual void handleEvent(sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};

class SceneManager {
private:
    Scene* currentScene;
    Scene* nextScene;
    bool needsTransition;
    
    // Transition effect
    sf::RectangleShape fadeShape;
    float fadeAlpha;
    bool isFadingOut;
    bool isFadingIn;
    float fadeSpeed;

public:
    SceneManager(sf::RenderWindow& window);
    ~SceneManager();

    void changeScene(Scene* newScene);
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void update(float dt, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    
    bool isTransitioning() const { return isFadingIn || isFadingOut; }
};

#endif
