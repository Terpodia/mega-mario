#include "GameEngine.h"
#include "EntityManager.h"
#include <fstream>

GameEngine::GameEngine(const std::string path) { init(path); }

void GameEngine::init(const std::string& path) {
    std::ifstream fin(path);
    std::string type;
    while (fin >> type) {
        if (type == "Texture") {
            std::string name, path;
            Vec2 size;
            fin >> name >> path >> size.x >> size.y;
            m_assets.addTexture(name, path, size);
        } else if (type == "Animation") {
            std::string animName, texName;
            size_t frameCount, speed;
            fin >> animName >> texName >> frameCount >> speed;
            m_assets.addAnimation(animName, texName, frameCount, speed);
        } else {
            std::string name, path;
            fin >> name >> path;
            m_assets.addFont(name, path);
        }
    }
    m_window.create(sf::VideoMode(1920, 1080), "mega mario",
                    sf::Style::Fullscreen);
}

void GameEngine::run() {
    EntityManager manager;
    auto e = manager.addEntity("player");
    e->addComponent<CAnimation>(m_assets.getAnimation("Run"));
    while (m_window.isOpen()) {
        update();
        m_window.draw(e->getComponent<CAnimation>().animation.getSprite());
        e->getComponent<CAnimation>().animation.update();
        m_window.display();
    }
}

void GameEngine::sUserInput() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) m_window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) m_window.close();
        }
    }
}

void GameEngine::update() {
    m_window.clear(sf::Color(0, 87, 217));
    sUserInput();
}

void GameEngine::quit() {
    m_window.close();
    m_running = false;
}

sf::RenderWindow& GameEngine::window() { return m_window; }

const Assets& GameEngine::assets() const { return m_assets; }

bool GameEngine::isRunning() const { return m_running; }
