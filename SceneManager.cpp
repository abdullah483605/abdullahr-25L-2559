#include "../include/SceneManager.h"

SceneManager::SceneManager(sf::RenderWindow& window) {
    currentScene = nullptr;
    nextScene = nullptr;
    needsTransition = false;
    
    fadeAlpha = 0.0f;
    isFadingOut = false;
    isFadingIn = false;
    fadeSpeed = 500.0f; // Alpha units per second
    
    sf::Vector2u size = window.getSize();
    fadeShape.setSize(sf::Vector2f(size.x, size.y));
    fadeShape.setFillColor(sf::Color(0, 0, 0, 0));
}

SceneManager::~SceneManager() {
    if (currentScene) delete currentScene;
    if (nextScene) delete nextScene;
}

void SceneManager::changeScene(Scene* newScene) {
    if (currentScene == nullptr) {
        currentScene = newScene;
        currentScene->init();
        isFadingIn = true;
        fadeAlpha = 255.0f;
    } else {
        nextScene = newScene;
        isFadingOut = true;
        fadeAlpha = 0.0f;
    }
}

void SceneManager::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (currentScene && !isFadingOut && !isFadingIn) {
        currentScene->handleEvent(event, window);
    }
}

void SceneManager::update(float dt, sf::RenderWindow& window) {
    // Handle transition
    if (isFadingOut) {
        fadeAlpha += fadeSpeed * dt;
        if (fadeAlpha >= 255.0f) {
            fadeAlpha = 255.0f;
            isFadingOut = false;
            
            // Switch scene
            delete currentScene;
            currentScene = nextScene;
            nextScene = nullptr;
            
            if (currentScene) {
                currentScene->init();
                isFadingIn = true;
            }
        }
        fadeShape.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeAlpha)));
    } else if (isFadingIn) {
        fadeAlpha -= fadeSpeed * dt;
        if (fadeAlpha <= 0.0f) {
            fadeAlpha = 0.0f;
            isFadingIn = false;
        }
        fadeShape.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeAlpha)));
    }
    
    if (currentScene) {
        currentScene->update(dt);
    }
}

void SceneManager::draw(sf::RenderWindow& window) {
    if (currentScene) {
        currentScene->draw(window);
    }
    
    if (isFadingIn || isFadingOut) {
        window.draw(fadeShape);
    }
}
