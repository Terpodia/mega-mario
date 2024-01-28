#include "Assets.h"

Assets::Assets() {}

void Assets::addTexture(std::string name, std::string path, Vec2 size) {
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load file at " << path << "\n";
        exit(-1);
    }
    m_textures[name] = {texture, size};
}

void Assets::addAnimation(const std::string animName,
                          const std::string textureName) {
    m_animations[animName] = Animation(animName, getTexture(textureName));
}

void Assets::addAnimation(const std::string animName,
                          const std::string textureName, size_t frameCount,
                          size_t speed) {
    m_animations[animName] =
        Animation(animName, getTexture(textureName), frameCount, speed);
}

void Assets::addFont(std::string name, std::string path) {
    sf::Font font;
    if (!font.loadFromFile(path)) {
        std::cerr << "Failed to load file at path " << path << "\n";
        exit(-1);
    }
    m_fonts[name] = font;
}

const Texture& Assets::getTexture(std::string name) { return m_textures[name]; }

const Animation& Assets::getAnimation(std::string name) {
    return m_animations[name];
}

const sf::Font& Assets::getFont(std::string name) { return m_fonts[name]; }
