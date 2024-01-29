#include "Scene_Play.h"
#include "GameEngine.h"

Scene_Play::Scene_Play(GameEngine* gameEngine, std::string levelPath) {
    m_game = gameEngine;
    init(levelPath);
}

void Scene_Play::init(const std::string levelPath) {
    registerAction(sf::Keyboard::Escape, "Quit");
}

void Scene_Play::update() {
    sRender();
}

void Scene_Play::sDoAction(const Action& action) {
    if(action.name() == "Quit") onEnd();
}

void Scene_Play::sRender() {
    m_game->window().clear(sf::Color(0, 87, 217));
    drawLine(Vec2(0, 0), Vec2(500, 500));
    m_game->window().display();
}

void Scene_Play::onEnd() {
    m_game->window().close();
}