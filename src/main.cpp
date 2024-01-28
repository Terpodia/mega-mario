#include <iostream>
#include "Animation.h"
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
  assets.addTexture("run", "../bin/assets/megaman/run.png", Vec2(192, 64));
  assets.addTexture("coin", "../bin/assets/others/coin.png", Vec2(192, 64));
  assets.addAnimation("run", "run", 3, 1000);
  assets.addFont("mario", "../bin/fonts/super-mario-bros-nes.ttf");

  Animation anim = assets.getAnimation("run");
  anim.getSprite().setPosition(300, 300);

  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event)) {
      if(event.type == sf::Event::Closed) window.close();
      if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::Escape) window.close();
      }
    }
    window.clear(sf::Color(0, 87, 217));

    sf::Text text("poseemos assets", assets.getFont("mario"), 34);
    anim.update();
    text.setPosition(500, 500);
    window.draw(text);
    window.draw(anim.getSprite());
    window.display();
  }
  return 0;
}
