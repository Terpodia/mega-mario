#include <iostream>
#include "EntityManager.h"

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

  return 0;
}
