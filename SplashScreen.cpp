#include "../include/SplashScreen.h"
#include "../include/LoginScreen.h"
#include "../include/UIManager.h"

using namespace std;

SplashScreen::SplashScreen() { displayTime = 1.0f; }
SplashScreen::~SplashScreen() {}

void SplashScreen::init() {
    UIManager* ui = UIManager::getInstance();
    sf::Font&  font = ui->getFont();
    sf::Vector2u ws = ui->getWindow()->getSize();
    float W=(float)ws.x, H=(float)ws.y;

    // Dark blue background
    bg.setSize(sf::Vector2f(W,H));
    bg.setFillColor(sf::Color(15,52,96));

    // Decorative circles
    float radii[5]={260,180,120,70,40};
    sf::Color cols[5]={
        sf::Color(41,128,185,25), sf::Color(52,152,219,20),
        sf::Color(255,255,255,10), sf::Color(41,128,185,30), sf::Color(52,152,219,15)};
    float cx[5]={W/2.f, W*0.1f, W*0.85f, W*0.2f, W*0.8f};
    float cy[5]={H/2.f, H*0.1f, H*0.9f, H*0.8f, H*0.15f};
    for (int i=0;i<5;i++) {
        decoCircle[i].setRadius(radii[i]);
        decoCircle[i].setFillColor(cols[i]);
        decoCircle[i].setOrigin(radii[i],radii[i]);
        decoCircle[i].setPosition(cx[i],cy[i]);
    }

    // Hospital cross
    crossH.setSize(sf::Vector2f(90,28));
    crossH.setFillColor(sf::Color(255,255,255,200));
    crossH.setOrigin(45,14);
    crossH.setPosition(W/2.f, H/2.f - 80);

    crossV.setSize(sf::Vector2f(28,90));
    crossV.setFillColor(sf::Color(255,255,255,200));
    crossV.setOrigin(14,45);
    crossV.setPosition(W/2.f, H/2.f - 80);

    crossBg.setRadius(68);
    crossBg.setFillColor(sf::Color(41,128,185,80));
    crossBg.setOutlineThickness(3);
    crossBg.setOutlineColor(sf::Color(255,255,255,60));
    crossBg.setOrigin(68,68);
    crossBg.setPosition(W/2.f, H/2.f - 80);

    // Title
    titleText.setFont(font);
    titleText.setString("MediCore");
    titleText.setCharacterSize(72);
    titleText.setFillColor(sf::Color::White);
    sf::FloatRect tr=titleText.getLocalBounds();
    titleText.setOrigin(tr.left+tr.width/2.f, tr.top+tr.height/2.f);
    titleText.setPosition(W/2.f, H/2.f+20);

    subtitleText.setFont(font);
    subtitleText.setString("Hospital Management System");
    subtitleText.setCharacterSize(22);
    subtitleText.setFillColor(sf::Color(189,215,238));
    sf::FloatRect sr=subtitleText.getLocalBounds();
    subtitleText.setOrigin(sr.left+sr.width/2.f, sr.top+sr.height/2.f);
    subtitleText.setPosition(W/2.f, H/2.f+75);

    tagline.setFont(font);
    tagline.setString("\"Your Health, Our Priority\"");
    tagline.setCharacterSize(16);
    tagline.setFillColor(sf::Color(149,165,166));
    sf::FloatRect tl=tagline.getLocalBounds();
    tagline.setOrigin(tl.left+tl.width/2.f,tl.top+tl.height/2.f);
    tagline.setPosition(W/2.f, H/2.f+110);

    // Progress bar background
    progressBg.setSize(sf::Vector2f(300,6));
    progressBg.setFillColor(sf::Color(255,255,255,40));
    progressBg.setOrigin(150,3);
    progressBg.setPosition(W/2.f, H*0.82f);

    progressBar.setSize(sf::Vector2f(0,6));
    progressBar.setFillColor(sf::Color(52,152,219));
    progressBar.setPosition(W/2.f-150, H*0.82f-3);

    loadingText.setFont(font);
    loadingText.setString("Loading...");
    loadingText.setCharacterSize(15);
    loadingText.setFillColor(sf::Color(149,165,166));
    sf::FloatRect lt=loadingText.getLocalBounds();
    loadingText.setOrigin(lt.left+lt.width/2.f,0);
    loadingText.setPosition(W/2.f, H*0.82f+14);

    timer.restart();
}

void SplashScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (event.type==sf::Event::KeyPressed || event.type==sf::Event::MouseButtonPressed) {
        if (displayTime < 9999.0f) {
            displayTime = 99999.0f; // Prevent multiple triggers
            UIManager::getInstance()->getSceneManager()->changeScene(new LoginScreen());
        }
    }
}

void SplashScreen::update(float dt) {
    float elapsed = timer.getElapsedTime().asSeconds();
    float progress = elapsed / displayTime;
    if (progress > 1.f) progress = 1.f;

    progressBar.setSize(sf::Vector2f(300.f*progress, 6));

    // Pulse the cross
    float scale = 1.f + 0.04f*sin(elapsed*3.f);
    crossBg.setScale(scale, scale);
    crossH.setScale(1.f,1.f);
    crossV.setScale(1.f,1.f);

    if (elapsed > displayTime) {
        displayTime = 99999.0f; // Prevent re-triggering every frame during fade
        UIManager::getInstance()->getSceneManager()->changeScene(new LoginScreen());
    }
}

void SplashScreen::draw(sf::RenderWindow& window) {
    window.draw(bg);
    for (int i=0;i<5;i++) window.draw(decoCircle[i]);

    window.draw(crossBg);
    window.draw(crossH);
    window.draw(crossV);

    window.draw(titleText);
    window.draw(subtitleText);
    window.draw(tagline);

    window.draw(progressBg);
    window.draw(progressBar);
    window.draw(loadingText);
}
