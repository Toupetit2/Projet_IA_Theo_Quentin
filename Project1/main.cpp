#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Grid.hpp"
#include "EnemyBehaviour.hpp"
#include <vector>

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

vector<shared_ptr<EnemyBehaviour>> vectorEnemyBehaviour;
vector<shared_ptr<Entity>> allEntity;
sf::Vector2i start(100, 500);


int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jeu SFML - IA Ennemis");
    window.setFramerateLimit(60);

    Player player(200, 200);
    std::vector<Enemy> enemies = { Enemy(100, 100), Enemy(700, 100) };
    shared_ptr<EnemyBehaviour> enemy_B1 = make_shared<EnemyBehaviour>("Fred", 100, 300, 70.f, 35.f, start); //std::string n, float x, float y, float circleDetect, float circleRange, sf::Vector2i start
    vectorEnemyBehaviour.push_back(enemy_B1);
    
    Grid grid;
    grid.loadFromFile("map2.txt");

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.update(deltaTime, grid, player);
        for (auto& enemy : enemies) {
            enemy.update(deltaTime, grid, player);
        }

        window.clear();


        grid.draw(window);
        window.draw(player.shape);
        for (const auto& enemy : enemies)
            window.draw(enemy.shape);

        enemy_B1->PlayerDetected(player);
        enemy_B1->PlayerInRange(player);
        enemy_B1->Patrolling();
        enemy_B1->update(deltaTime, grid, player);
        enemy_B1->draw(window);

        window.display();
    }
    return 0;
}

