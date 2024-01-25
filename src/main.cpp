#include <iostream>
#include <Vec2.h>

int main() {
  Vec2 v(3, 2);
  v *= 3;
  std::cout << v.x << " " << v.y << "\n";
  return 0;
}
