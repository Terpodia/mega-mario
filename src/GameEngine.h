#pragma once

#include "Assets.h"

class GameEngine {
   protected:
    sf::RenderWindow m_window;
    Assets m_assets;
    bool m_running = true;

    void init(const std::string& path);
    void update();

    void sUserInput();

   public:
    GameEngine(const std::string path);
    void run();
    void quit();
    sf::RenderWindow& window();
    const Assets& assets() const;
    bool isRunning() const;
};
