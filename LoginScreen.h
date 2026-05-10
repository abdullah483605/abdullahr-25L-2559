#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "SceneManager.h"
#include "Button.h"
#include "InputBox.h"
#include "Label.h"

class LoginScreen : public Scene {
private:
    // Background layers
    sf::RectangleShape bgLeft;    // deep blue panel
    sf::RectangleShape bgRight;   // white/light panel

    // Hospital cross logo (drawn with rectangles)
    sf::RectangleShape crossH, crossV;
    sf::CircleShape    logoBg;

    // Branding text
    sf::Text  brandTitle;
    sf::Text  brandSubtitle;
    sf::Text  tagline;

    // Login card
    sf::RectangleShape card;
    sf::RectangleShape cardHeader;
    sf::Text           cardHeaderText;
    sf::Text           cardSubText;

    // Role selector pills
    sf::RectangleShape rolePill[3];
    sf::Text           roleText[3];
    int selectedRole;

    // Decorative circles (background art)
    sf::CircleShape decorCircle[4];

    // Form fields
    sf::RectangleShape idIcon, passIcon;
    sf::Text           idIconText, passIconText;
    InputBox*          idInput;
    InputBox*          passInput;

    // Buttons
    Button*   loginBtn;
    Button*   exitBtn;

    // Error label
    Label*    errorLabel;
    Label*    attemptsLabel;

    // Animation
    sf::Clock animClock;
    float     pulseVal;

    int failedAttempts;

    void drawBackground(sf::RenderWindow& window);
    void drawLogo(sf::RenderWindow& window);
    void drawRolePills(sf::RenderWindow& window);
    void attemptLogin();

    // Patient Registration elements
    bool isRegistering;
    Button* openRegBtn;
    InputBox* regFields[6];
    Label* regLabels[6];
    Button* regSubmitBtn;
    Button* regCancelBtn;
    void attemptRegister();

public:
    LoginScreen();
    ~LoginScreen() override;

    void init() override;
    void handleEvent(sf::Event& event, sf::RenderWindow& window) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
};

#endif
