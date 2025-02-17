#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

class Enemy : public Entity {
private:

public:
    float SPEED = 1.0f;
    Enemy(float x, float y);

    void update(float deltaTime, Grid& grid, Entity& player) override;

    void chase(Vector2f pPos, float deltaTime);
    void patrol(Vector2f ePos);
    //void search(float deltaTime);

};

#endif // ENEMY_HPP