#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

#include "Texture.h"
#include "Animation.h"
#include "Vec2.h"

class Assets {
    std::map<std::string, Texture> m_textures;
    std::map<std::string, sf::Font> m_fonts;
    std::map<std::string, Animation> m_animations;

   public:
    Assets();

    void addTexture(const std::string name, const std::string path, Vec2 size);

    void addAnimation(const std::string animName,
                      const std::string textureName);

    void addAnimation(const std::string animName, const std::string textureName,
                      size_t frameCount, size_t speed);

    void addFont(const std::string name, const std::string path);

    const Texture& getTexture(const std::string name);
    const Animation& getAnimation(const std::string name);
    const sf::Font& getFont(const std::string name);
};
