#include <fstream>
#include <iostream>

#include "Assets.h"
#include "EntityManager.h"

int main() {
    Assets assets;

    std::ifstream fin("../bin/assets.txt");
    std::string type;
    while (fin >> type) {
        if (type == "Texture") {
            std::string name;
            std::string path;
            Vec2 size;
            fin >> name >> path >> size.x >> size.y;
            assets.addTexture(name, path, size);
        } else if (type == "Animation") {
            std::string animName;
            std::string texName;
            size_t frameCount, speed;
            fin >> animName >> texName >> frameCount >> speed;
            assets.addAnimation(animName, texName, frameCount, speed);
        } else {
            std::string name;
            std::string path;
            fin >> name >> path;
            assets.addFont(name, path);
        }
    }

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "mega mario",
                            sf::Style::Fullscreen);

    EntityManager manager;
    auto e = manager.addEntity("player");
    manager.update();

    Animation anim = assets.getAnimation("Run");
    anim.getSprite().setPosition(300, 300);

    e->addComponent<CAnimation>(anim);
    if (e->hasComponent<CAnimation>()) std::cout << "1\n";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) window.close();
            }
        }
        window.clear(sf::Color(0, 87, 217));

        e = manager.getEntities("player")[0];

        sf::Text text("poseemos assets", assets.getFont("MarioFont"), 34);
        e->getComponent<CAnimation>().animation.update();
        text.setPosition(500, 500);
        window.draw(text);
        window.draw(e->getComponent<CAnimation>().animation.getSprite());
        window.display();
    }
    return 0;
}
