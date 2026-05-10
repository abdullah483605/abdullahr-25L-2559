// =============================================================
// main.cpp - Entry point for SFML GUI
// MediCore Hospital Management System
// =============================================================
#include <SFML/Graphics.hpp>
#include "include/UIManager.h"
#include <iostream>

int main() {
    std::cout << "Starting MediCore GUI...\n";
    
    // Create SFML Window
    sf::RenderWindow window(sf::VideoMode(1024, 768), "MediCore - Hospital Management System", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Initialize UI Manager (loads backend files, sets up scenes)
    UIManager* ui = UIManager::getInstance();
    ui->init(&window);

    // Run the main GUI loop
    ui->run();

    // Clean up
    UIManager::cleanup();
    
    return 0;
}
