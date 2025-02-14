#include "EnemyFSM.hpp"

EnemyFSM::EnemyFSM(float x, float y) : Enemy(x, y) {}

void EnemyFSM::update(float deltaTime, Grid& grid, Vector2f playerPos)
{
    /*
    switch (currentState) {
    case PATROL:
        patrol();
        if (detectPlayer(playerPos)) currentState = CHASE;
        break;

    case CHASE:
        chase(playerPos);
        if (!detectPlayer(playerPos)) {
            currentState = SEARCH;
        }
        break;

    case SEARCH:
        break;
    }
    */
}