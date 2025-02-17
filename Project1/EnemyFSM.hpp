#include <iostream>
#include <vector>
#include "Enemy.hpp"

using namespace std;
using namespace sf;

class EnemyFSM : public Enemy
{
private:
	enum State { PATROL, CHASE, SEARCH };
	State currentState = PATROL;
	float detectionRange = 200;
	float searchTime = 5.f;

	Vector2f lastPlayerPos;
	float timeSinceSearchStarted = 0.f;
	float lastDirectionChangeTime = 0.f;
	Vector2f searchDirection;
	float searchRange = 100;

	vector<Vector2f> patrolTargetPositions = { Vector2f(10, 10), Vector2f(300, 50) };
	int currentTargetID = 0;

	//debug
	State lastState = SEARCH;
public:
	EnemyFSM(float x, float y);
	void update(float deltaTime, Grid& grid, Entity& player) override;

	bool detectPlayer(Vector2f pPos);

	bool collisionWithWall(Grid& grid);

	void search(Vector2f lastPlayerPosition, float deltaTime, Grid& grid);
};