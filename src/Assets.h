#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

class Assets {
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::Font> m_fonts;

   public:
    Assets();

    void addTexture(const std::string name, const std::string path);
    void addFont(const std::string name, const std::string path);

    const sf::Texture& getTexture(const std::string name);
    const sf::Font& getFont(const std::string name);
};
