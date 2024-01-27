#include <iostream>
#include "EntityManager.h"
#include "Assets.h"

int main() {
  EntityManager manager; 
  auto e = manager.addEntity("player");
  manager.update();

  std::cout << e->tag() << " " << e->id() << "\n";

  e->addComponent<CTransform>(Vec2(3, 8) * 2);
  Vec2& pos = e->getComponent<CTransform>().pos;
  pos.x++, pos.y++;

  std::cout << e->getComponent<CTransform>().pos.x << " " << e->getComponent<CTransform>().pos.y << "\n";

  e->removeComponent<CTransform>();

  if(e->hasComponent<CTransform>()) std::cout << e->hasComponent<CTransform>() << "\n";
  //if(e->hasComponent<CBoundingBox>()) std::cout << e->hasComponent<CBoundingBox>() << "\n";

  sf::RenderWindow window(sf::VideoMode(1920, 1080), "mega mario", sf::Style::Fullscreen);

  Assets assets;
  assets.addTexture("ground", "../bin/assets/mario/ground.png");
  assets.addTexture("brick", "../bin/assets/mario/brick.png");
  assets.addFont("mario", "../bin/fonts/super-mario-bros-nes.ttf");
  assets.addTexture("pipe", "../bin/assets/mario/tall pipe.png");

  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event)) {
      if(event.type == sf::Event::Closed) window.close();
    }
    window.clear(sf::Color(0, 87, 217));
    sf::Texture tex = assets.getTexture("ground");
    sf::Sprite sprite(tex);
    sprite.setScale(64.0 / tex.getSize().x, 64.0 / tex.getSize().y); 

    sf::Texture tex2 = assets.getTexture("brick");
    sf::Sprite sprite2(tex2);
    sprite2.setScale(64.0 / tex2.getSize().x, 64.0 / tex2.getSize().y); 
    sprite2.setPosition(300, 300);

    sf::Text text("poseemos assets", assets.getFont("mario"), 34);
    text.setPosition(500, 500);

    sf::Texture tex3 = assets.getTexture("pipe");
    sf::Sprite sprite3(tex3);
    sprite3.setScale(128.0 / tex3.getSize().x, 128.0 / tex3.getSize().x); 
    sprite3.setPosition(800, 800);

    window.draw(sprite);
    window.draw(sprite2);
    window.draw(sprite3);
    window.draw(text);
    window.display();
  }
  return 0;
}
