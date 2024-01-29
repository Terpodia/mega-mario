#pragma once

#include "Scene.h"

class Scene_Play : public Scene {
    struct PlayerConfig {
        float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
        std::string WEAPON;
    };

   protected:
    std::shared_ptr<Entity> m_player;
    std::string m_levelPath;
    PlayerConfig m_playerConfing;
    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;
    const Vec2 m_gridSize = {64, 64};
    void init(std::string levelPath);
    void onEnd();

   public:
    Scene_Play(GameEngine* gameEngine = nullptr, std::string levelPath = "");
    void update();
    void sDoAction(const Action& action);
    void sRender();
    void sAnimation();
    void sMovement();

    float getPlayerDirection();
    void flipPlayer();
};
