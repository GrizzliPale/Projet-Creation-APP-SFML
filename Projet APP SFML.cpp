#include <SFML/Graphics.hpp>
#include <windows.h>
#include <deque>

#include <cstdlib>

#include <ctime>


class Snake {

public:

    Snake(int block_size, int width, int height)

        : block_size(block_size), width(width), height(height), direction(sf::Vector2i(1, 0)) {

        reset();

    }

    void reset() {

        body.clear();

        body.push_front(sf::Vector2i(5, 5));

        for (int i = 1; i < 5; i++) {

            body.push_back(sf::Vector2i(5 - i, 5));

        }

        direction = sf::Vector2i(1, 0);

    }

    void move() {

        sf::Vector2i new_head = body.front() + direction;



        if (new_head.x < 0) new_head.x = width - 1;

        if (new_head.y < 0) new_head.y = height - 1;

        if (new_head.x >= width) new_head.x = 0;

        if (new_head.y >= height) new_head.y = 0;



        body.push_front(new_head);

        body.pop_back();

    }

    void grow() {

        body.push_back(body.back());

    }

    void setDirection(const sf::Vector2i& new_direction) {

        if (new_direction + direction != sf::Vector2i(0, 0)) {

            direction = new_direction;

        }

    }

    bool checkCollision() {

        for (size_t i = 1; i < body.size(); i++) {

            if (body[i] == body.front()) {

                return true;

            }

        }

        return false;

    }

    void draw(sf::RenderWindow& window) {

        for (auto& segment : body) {

            sf::RectangleShape rect(sf::Vector2f(block_size, block_size));

            rect.setPosition(segment.x * block_size, segment.y * block_size);

            rect.setFillColor(sf::Color::Red);

            window.draw(rect);

        }

    }



    const sf::Vector2i& getHead() const {

        return body.front();

    }

private:

    int block_size;

    int width, height;

    std::deque<sf::Vector2i> body;

    sf::Vector2i direction;

};

class Food {

public:

    Food(int block_size, int width, int height)

        : block_size(block_size), width(width), height(height) {

        spawn();

    }

    void spawn() {

        food_position.x = rand() % width;

        food_position.y = rand() % height;

    }

    void draw(sf::RenderWindow& window) {

        sf::RectangleShape rect(sf::Vector2f(block_size, block_size));

        rect.setPosition(food_position.x * block_size, food_position.y * block_size);

        rect.setFillColor(sf::Color::Green);

        window.draw(rect);

    }

    const sf::Vector2i& getPosition() const {

        return food_position;

    }

private:

    int block_size;

    int width, height;

    sf::Vector2i food_position;

};

int main() {

    srand(static_cast<unsigned int>(time(0)));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Snake Game");

    window.setFramerateLimit(10);


    int block_size = 20;

    int grid_width = window.getSize().x / block_size;

    int grid_height = window.getSize().y / block_size;


    Snake snake(block_size, grid_width, grid_height);

    Food food(block_size, grid_width, grid_height);

    sf::Clock clock;

    bool game_over = false;

    sf::Font font;

    /*if (!font.loadFromFile("arial.ttf")) {

        return -1; // Charger une police pour afficher du texte

    }*/

    sf::Text game_over_text;

    game_over_text.setFont(font);

    game_over_text.setString("Game Over! Press R to Restart");

    game_over_text.setCharacterSize(30);

    game_over_text.setFillColor(sf::Color::White);

    game_over_text.setPosition(150, 250);

    while (window.isOpen()) {

        sf::Time deltaTime = clock.restart();

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)

                window.close();

            if (event.type == sf::Event::KeyPressed) {

                if (!game_over) {

                    if (event.key.code == sf::Keyboard::Up) {

                        snake.setDirection(sf::Vector2i(0, -1));

                    }

                    if (event.key.code == sf::Keyboard::Down) {

                        snake.setDirection(sf::Vector2i(0, 1));

                    }

                    if (event.key.code == sf::Keyboard::Left) {

                        snake.setDirection(sf::Vector2i(-1, 0));

                    }

                    if (event.key.code == sf::Keyboard::Right) {

                        snake.setDirection(sf::Vector2i(1, 0));

                    }

                }
                else {

                    if (event.key.code == sf::Keyboard::R) {

                        game_over = false;

                        snake.reset();

                        food.spawn();

                    }

                }

            }

        }

            if (!game_over) {

            snake.move();

            if (snake.getHead() == food.getPosition()) {

                snake.grow();

                food.spawn();

            }

            if (snake.checkCollision()) {

                game_over = true;

            }

        }

        window.clear();

        // Dessiner le fond

        //drawBackground(window);

        // Dessiner les objets

        snake.draw(window);

        food.draw(window);

        if (game_over) {

            window.draw(game_over_text);

        }

        window.display();

    }

    return 0;

}
