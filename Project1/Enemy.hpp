#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

class Enemy : public Entity {
private:

public:
    static constexpr float SPEED = 1.0f;
    Enemy(float x, float y);

    void update(float deltaTime, Grid& grid, Entity& playerPos) override;

    void chase(Vector2f pPos);
    void patrol();
    //void search(float deltaTime);

};

#endif // ENEMY_HPP