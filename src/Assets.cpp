#include "Assets.h"

Assets::Assets() {}

void Assets::addTexture(std::string name, std::string path) {
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load file at " << path << "\n";
        exit(-1);
    }
    m_textures[name] = texture;
}

void Assets::addFont(std::string name, std::string path) {
    sf::Font font;
    if (!font.loadFromFile(path)) {
        std::cerr << "Failed to load file at path " << path << "\n";
        exit(-1);
    }
    m_fonts[name] = font;
}

const sf::Texture& Assets::getTexture(std::string name) {
    return m_textures[name];
}

const sf::Font& Assets::getFont(std::string name) { return m_fonts[name]; }
