#include <iostream>
#include "Enemy.hpp"

using namespace std;
using namespace sf;

class EnemyFSM : public Enemy
{
private:
	enum State { PATROL, CHASE, SEARCH };
	State currentState;
public:
	EnemyFSM(float x, float y);
	void update(float deltaTime, Grid& grid, Vector2f playerPos) override;
	void patrol();
	void chase();
};