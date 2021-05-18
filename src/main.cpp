#include "core/World.hpp"
#include "objects/Ball.hpp"
#include "objects/Box.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 500), "SimpleNgi");
    window.setFramerateLimit(60);
    World wrld;

    Box ground(&wrld, Vec2(400, 50), Vec2(200, 400));
    ground.setAnchored(true);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    new Box(&wrld, Vec2(20, 20), (Vec2)sf::Mouse::getPosition(window));
                } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    new Ball(&wrld, 20.0f, (Vec2)sf::Mouse::getPosition(window));
                }
            }
        }

        // tick
        wrld.tick();

        // render everything
        window.clear(sf::Color::White);
        wrld.render(window);
        window.display();
    }

    return 0;
}
