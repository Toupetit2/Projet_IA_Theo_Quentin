#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Enemy.hpp"

class Player : public Entity {
public:
    float SPEED = 200.0f;
    int DAMAGE = 20;
    float ATTACK_COOLDOWN = 0.5f;
    float attackTimer;

    Player(float x, float y, int hp);
    //void attack(std::vector<Entity*> enemies);
    void attack(Entity& enemy);
    void update(float deltaTime, Grid& grid, Entity& enemy) override;

};

#endif // PLAYER_HPP