#include "../include/UIManager.h"
#include "../include/SplashScreen.h"
#include <iostream>

using namespace std;

UIManager* UIManager::instance = nullptr;

UIManager::UIManager() {
    window = nullptr;
    sceneManager = nullptr;
    fileHandler = new FileHandler("data/");
    validator = new Validator();
    loggedInUserID = nullptr;
    userRole = nullptr;
}

UIManager::~UIManager() {
    delete sceneManager;
    delete fileHandler;
    delete validator;
    if (loggedInUserID) delete[] loggedInUserID;
}

UIManager* UIManager::getInstance() {
    if (instance == nullptr) {
        instance = new UIManager();
    }
    return instance;
}

void UIManager::cleanup() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

void UIManager::init(sf::RenderWindow* win) {
    window = win;
    sceneManager = new SceneManager(*window);
    
    // Load a default Windows font since SFML requires a font
    if (!mainFont.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cout << "Failed to load font!\n";
    }
    
    loadData();
    
    // Start with splash screen
    sceneManager->changeScene(new SplashScreen());
}

void UIManager::loadData() {
    // Load all data from files into memory storage
    fileHandler->loadPatients(&patients);
    fileHandler->loadDoctors(&doctors);
    fileHandler->loadAdmins(&admins);
    fileHandler->loadAppointments(&appointments);
    fileHandler->loadBills(&bills);
    fileHandler->loadPrescriptions(&prescriptions);
}

void UIManager::login(const char* id, const char* role) {
    if (loggedInUserID) delete[] loggedInUserID;
    loggedInUserID = allocCopy(id);
    userRole = role; // static strings passed like "Patient"
}

void UIManager::logout() {
    if (loggedInUserID) {
        delete[] loggedInUserID;
        loggedInUserID = nullptr;
    }
    userRole = nullptr;
}

void UIManager::run() {
    sf::Clock clock;
    
    while (window->isOpen()) {
        float dt = clock.restart().asSeconds();
        
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            sceneManager->handleEvent(event, *window);
        }
        
        sceneManager->update(dt, *window);
        
        window->clear(sf::Color(245, 245, 245)); // Light background
        sceneManager->draw(*window);
        window->display();
    }
}
