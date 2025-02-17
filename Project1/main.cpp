#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Grid.hpp"
#include <vector>

#include "EnemyFSM.hpp"


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jeu SFML - IA Ennemis");
    window.setFramerateLimit(60);

    Player player(200, 400);
    std::vector<shared_ptr<Enemy>> vectorEnemy;
    shared_ptr<EnemyFSM> enemy1 = make_shared<EnemyFSM>(EnemyFSM(100, 100));
    vectorEnemy.push_back(enemy1);
    enemy1 = make_shared<EnemyFSM>(EnemyFSM(700, 100));
    vectorEnemy.push_back(enemy1);

    Grid grid;
    grid.loadFromFile("map.txt");

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
        for (auto& enemy : vectorEnemy) {
            std::shared_ptr<EnemyFSM> fsm = std::dynamic_pointer_cast<EnemyFSM>(enemy);
            if (fsm) {
                fsm->update(deltaTime, grid, player);
            }
        }

        window.clear();
        grid.draw(window);
        window.draw(player.shape);
        for (const auto& enemy : vectorEnemy)
            window.draw(enemy->shape);
        window.display();
    }
    return 0;
}

