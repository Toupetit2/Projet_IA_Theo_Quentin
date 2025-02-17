#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "Pathfinding.hpp"
#include "Entity.hpp"

class Enemy : public Entity {
private:

public:
    static constexpr float SPEED = 100.0f;
    Enemy(float x, float y);

    void update(float deltaTime, Grid& grid, Entity& playerPos) override;

    void chase(Vector2f pPos, float deltaTime, Grid& grid);
    void patrol(float deltaTime);
    //void search(float deltaTime);

};

#endif // ENEMY_HPP