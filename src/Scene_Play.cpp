#include "Scene_Play.h"

#include <fstream>

#include "GameEngine.h"

#include "Physics.h"

Scene_Play::Scene_Play(GameEngine* gameEngine, std::string levelPath) {
  m_game = gameEngine;
  init(levelPath);
}

void Scene_Play::init(const std::string& levelPath) {
  registerAction(sf::Keyboard::Escape, "Quit");
  registerAction(sf::Keyboard::D, "Right");
  registerAction(sf::Keyboard::A, "Left");
  registerAction(sf::Keyboard::F, "Draw Grid");
  registerAction(sf::Keyboard::J, "Draw Collision");

  m_gridText.setFont(m_game->assets().getFont("TechFont"));
  m_gridText.setCharacterSize(12);

  std::ifstream fin(levelPath);
  std::string type;
  while (fin >> type) {
    if (type == "Dec") {
      std::string name;
      int x, y;
      fin >> name >> x >> y;
      addDecoration(x, y, name);
    } else if (type == "Tile") {
      std::string name;
      int x, y;
      fin >> name >> x >> y;
      addTile(x, y, name);
    } else if (type == "Player") {
      fin >> m_playerConfing.X >> m_playerConfing.Y >> m_playerConfing.CX >>
          m_playerConfing.CY >> m_playerConfing.SPEED >>
          m_playerConfing.MAXSPEED >> m_playerConfing.JUMP >>
          m_playerConfing.GRAVITY >> m_playerConfing.WEAPON;
      addPlayer();
    }
  }
}

void Scene_Play::addDecoration(int x, int y, std::string name) {
  auto dec = m_entityManager.addEntity("decoration");
  auto& anim =
      dec->addComponent<CAnimation>(m_game->assets().getAnimation(name));

  auto& sprite = anim.animation.getSprite();

  mapGridToPixels(x, y);
  x += sprite.getOrigin().x * sprite.getScale().x;
  y -= sprite.getOrigin().y * sprite.getScale().y;

  dec->addComponent<CTransform>(Vec2(x, y));
}

void Scene_Play::addTile(int x, int y, std::string name) {
  auto tile = m_entityManager.addEntity("tile");
  auto& anim =
      tile->addComponent<CAnimation>(m_game->assets().getAnimation(name));

  auto& sprite = anim.animation.getSprite();

  mapGridToPixels(x, y);
  x += sprite.getOrigin().x * sprite.getScale().x;
  y -= sprite.getOrigin().y * sprite.getScale().y;

  tile->addComponent<CTransform>(Vec2(x, y));

  float cx = sprite.getOrigin().x * sprite.getScale().x * 2.0;
  float cy = sprite.getOrigin().y * sprite.getScale().y * 2.0;

  tile->addComponent<CBoundingBox>(Vec2(cx, cy));
}

void Scene_Play::addPlayer() {
  m_player = m_entityManager.addEntity("player");
  m_player->addComponent<CInput>();
  auto& anim = m_player->addComponent<CAnimation>(
      m_game->assets().getAnimation("Stand"));

  int x = m_playerConfing.X, y = m_playerConfing.Y;
  mapGridToPixels(x, y);
  x += anim.animation.getSprite().getOrigin().x *
       anim.animation.getSprite().getScale().x;
  y -= anim.animation.getSprite().getOrigin().y *
       anim.animation.getSprite().getScale().y;
  m_player->addComponent<CTransform>(Vec2(x, y));
  m_player->addComponent<CBoundingBox>(
      Vec2(m_playerConfing.CX, m_playerConfing.CY));
}

void Scene_Play::update() {
  m_entityManager.update();
  sAnimation();
  sMovement();
  sCollision();
  sRender();
}

void Scene_Play::sDoAction(const Action& action) {
  if (action.name() == "Quit") {
    onEnd();
    return;
  }

  if (action.type() == "START") {
    if (action.name() == "Right") {
      m_player->getComponent<CInput>().right = true;
    } else if (action.name() == "Left") {
      m_player->getComponent<CInput>().left = true;
    } else if (action.name() == "Draw Grid")
      m_drawGrid = !m_drawGrid;
    else if (action.name() == "Draw Collision")
      m_drawCollision = !m_drawCollision;
  }

  else {
    if (action.name() == "Right") {
      m_player->getComponent<CInput>().right = false;
    } else if (action.name() == "Left") {
      m_player->getComponent<CInput>().left = false;
    }
  }
}

float Scene_Play::getPlayerDirection() {
  float flip =
      m_player->getComponent<CAnimation>().animation.getSprite().getScale().x;
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

  if (!input.right && !input.left) {
    float flip = getPlayerDirection();
    auto& anim = m_player->getComponent<CAnimation>();
    anim.animation = m_game->assets().getAnimation("Stand");

    auto spriteScale = anim.animation.getSprite().getScale();
    anim.animation.getSprite().setScale(spriteScale.x * flip, spriteScale.y);
  }

  else {
    if (m_player->getComponent<CAnimation>().animation.getName() == "Stand")
      m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Run"));

    float flip = getPlayerDirection();

    if (input.right && flip < 0)
      flipPlayer();
    else if (input.left && flip > 0)
      flipPlayer();
  }
}

void Scene_Play::sMovement() {
  auto& input = m_player->getComponent<CInput>();
  Vec2 in(0, 0);
  if (input.right) in.x++;
  if (input.left) in.x--;
  m_player->getComponent<CTransform>().velocity =
      in.normalize() * m_playerConfing.SPEED;

  for (auto& e : m_entityManager.getEntities()) {
    if (e->hasComponent<CTransform>()) {
      auto& transform = e->getComponent<CTransform>();
      transform.prevPos = transform.pos;
      transform.pos += transform.velocity;
    }
  }
}

void Scene_Play::sCollision() {
  for(auto& e : m_entityManager.getEntities("tile")) {
    Vec2 overlap = Physics::GetOverlap(m_player, e);
    Vec2 prevOverlap = Physics::GetPreviousOverlap(m_player, e);
    Vec2& playerPos = m_player->getComponent<CTransform>().pos;
    Vec2& tilePos = e->getComponent<CTransform>().pos;

    if(overlap.x <= 0 || overlap.y <= 0) continue;

    if(prevOverlap.x > 0) {
      if(playerPos.y > tilePos.y) playerPos.y += overlap.y+1;
      else playerPos.y -= overlap.y+1;
    }
    else {
      if(playerPos.x > tilePos.x) playerPos.x += overlap.x+1;
      else playerPos.x -= overlap.x+1;
    }
  }
}

void Scene_Play::sRender() {
  m_game->window().clear(sf::Color(0, 87, 217));

  auto& pos = m_player->getComponent<CTransform>().pos;
  float windowCenterX = std::max(width() / 2.0f, pos.x);
  sf::View view = m_game->window().getView();
  view.setCenter(windowCenterX, height() - view.getCenter().y);
  m_game->window().setView(view);

  for (auto e : m_entityManager.getEntities()) {
    if (e->hasComponent<CTransform>() && e->hasComponent<CAnimation>()) {
      auto& animation = e->getComponent<CAnimation>().animation;
      auto& transform = e->getComponent<CTransform>();
      animation.update();
      animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
      m_game->window().draw(
          e->getComponent<CAnimation>().animation.getSprite());
    }
  }

  if (m_drawGrid) {
    float leftX = m_game->window().getView().getCenter().x - width() / 2.0;
    float rightX = leftX + width() + m_gridSize.x;
    float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);
    for (float x = nextGridX; x < rightX; x += m_gridSize.x)
      drawLine(Vec2(x, 0), Vec2(x, height()));
    for (float y = 0; y < height(); y += m_gridSize.y) {
      drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));
      for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
        std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
        std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
        m_gridText.setString("(" + xCell + "," + yCell + ")");
        m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 6);
        m_game->window().draw(m_gridText);
      }
    }
  }

  if(m_drawCollision) {
    for(auto& e : m_entityManager.getEntities()) {
      if (!e->hasComponent<CBoundingBox>()) continue;
      auto & box = e->getComponent<CBoundingBox>();
      auto & transform = e->getComponent<CTransform>();
      sf::RectangleShape rect;
      rect.setSize(sf::Vector2f(box.size.x, box.size.y));
      rect.setOrigin(box.halfSize.x, box.halfSize.y);
      rect.setPosition(transform.pos.x, transform.pos.y);
      rect.setFillColor(sf::Color(0,0,0,0));
      rect.setOutlineColor(sf::Color(255, 255, 255, 255));
      rect.setOutlineThickness(1);
      m_game->window().draw(rect);
    }
  }

  m_game->window().display();
}

void Scene_Play::mapGridToPixels(int& x, int& y) {
  x *= m_gridSize.x;
  y = m_game->window().getSize().y - m_gridSize.y * y;
}

void Scene_Play::onEnd() { m_game->window().close(); }
