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

	sf::Vector2f firstPosition{ 150, 100 };
	sf::Vector2f secondPosition{ 150, 300 };
	sf::Vector2f thridPosition{ 500, 300 };
	sf::Vector2f fourthPosition{ 300, 150 };

	//debug
	State lastState = SEARCH;
public:
	EnemyFSM(float x, float y, int hp);

	void update(float deltaTime, Grid& grid, Entity& player) override;

	bool detectPlayer(Vector2f pPos);

	bool collisionWithWall(Grid& grid);

	void patrol(Vector2f ePos, float deltaTime, sf::Vector2f& firstPoint, sf::Vector2f& secondPoint, sf::Vector2f& thirdPoint, sf::Vector2f& fourthPoint) override;

	void search(Vector2f lastPlayerPosition, float deltaTime, Grid& grid);
};