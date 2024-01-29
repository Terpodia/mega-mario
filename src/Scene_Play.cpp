#include "Scene_Play.h"
#include "GameEngine.h"

Scene_Play::Scene_Play(GameEngine* gameEngine, std::string levelPath) {
    m_game = gameEngine;
    init(levelPath);
}

void Scene_Play::init(const std::string levelPath) {
    registerAction(sf::Keyboard::Escape, "Quit");
    registerAction(sf::Keyboard::D, "Right");
    registerAction(sf::Keyboard::A, "Left");

    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CTransform>(Vec2(300, 300));
    m_player->addComponent<CInput>();
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"));
}

void Scene_Play::update() {
    m_entityManager.update();
    sAnimation();
    sMovement();
    sRender();
}

void Scene_Play::sDoAction(const Action& action) {
    if(action.name() == "Quit") {
        onEnd();
        return;
    }

    if (action.type() == "START") {
        if(action.name() == "Right") {
            m_player->getComponent<CInput>().right = true;
        }
        else if(action.name() == "Left") {
            m_player->getComponent<CInput>().left = true;
        }
    }

    else {
        if(action.name() == "Right") {
            m_player->getComponent<CInput>().right = false;
        }
        else if(action.name() == "Left") {
            m_player->getComponent<CInput>().left = false;
        }
    }
}

float Scene_Play::getPlayerDirection() {
    float flip = m_player->getComponent<CAnimation>().animation.getSprite().getScale().x;
    flip /= abs(flip);
    return flip;
}

void Scene_Play::flipPlayer() {
    auto& anim = m_player->getComponent<CAnimation>();
    auto spriteScale = anim.animation.getSprite().getScale();
    spriteScale.x *= -1;
    anim.animation.getSprite().setScale(spriteScale);
}

void Scene_Play::sAnimation() {
    auto& input = m_player->getComponent<CInput>();

    if(!input.right && !input.left) { 
        float flip = getPlayerDirection();
        auto& anim = m_player->getComponent<CAnimation>();
        anim.animation = m_game->assets().getAnimation("Stand");

        auto spriteScale = anim.animation.getSprite().getScale();
        anim.animation.getSprite().setScale(spriteScale.x * flip, spriteScale.y);
    }

    else {
        if(m_player->getComponent<CAnimation>().animation.getName() == "Stand") 
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Run"));

        float flip = getPlayerDirection();

        if(input.right && flip < 0) flipPlayer();
        else if(input.left && flip > 0) flipPlayer();
    }
}

void Scene_Play::sMovement() {
    auto& input = m_player->getComponent<CInput>();
    Vec2 in(0, 0);
    if(input.right) in.x++;
    if(input.left) in.x--;
    m_player->getComponent<CTransform>().velocity = in.normalize() * 7.0;

    for(auto& e : m_entityManager.getEntities()) {
        if(e->hasComponent<CTransform>()) {
            auto& transform = e->getComponent<CTransform>();
            transform.prevPos = transform.pos;
            transform.pos += transform.velocity;
        }
    }
}

void Scene_Play::sRender() {
    m_game->window().clear(sf::Color(0, 87, 217));
    for(auto e : m_entityManager.getEntities()) {
        if(e->hasComponent<CTransform>() && e->hasComponent<CAnimation>()) {
            auto& animation = e->getComponent<CAnimation>().animation;
            auto& transform = e->getComponent<CTransform>();
            animation.update();
            animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
            m_game->window().draw(e->getComponent<CAnimation>().animation.getSprite());
        }
    }
    m_game->window().display();
}

void Scene_Play::onEnd() {
    m_game->window().close();
}