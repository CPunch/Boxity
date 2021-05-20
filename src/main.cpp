#include "core/Root.hpp"
#include "objects/Ball.hpp"
#include "objects/Box.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 500), "SimpleNgi");
    window.setFramerateLimit(60);
    Root root;

    Box ground(&root, Vec2(400, 50), Vec2(200, 400));
    ground.setAnchored(true);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    Box *b = new Box(&root, Vec2(20, 20), (Vec2)sf::Mouse::getPosition(window));
                    b->setAnchored(true);
                } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    new Ball(&root, 10.0f, (Vec2)sf::Mouse::getPosition(window));
                }
            }
        }

        // tick
        root.tick();

        // render everything
        window.clear(sf::Color::White);
        root.render(window);
        window.display();
    }

    return 0;
}
