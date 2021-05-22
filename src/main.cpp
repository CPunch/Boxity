#include "core/Root.hpp"
#include "objects/Ball.hpp"
#include "objects/Box.hpp"

#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 500), "SimpleNgi");
    window.setFramerateLimit(60);
    std::shared_ptr<Root> root = std::make_shared<Root>();

    std::shared_ptr<Box> ground = std::make_shared<Box>(Vec2(400, 50), Vec2(200, 400));
    ground->setAnchored(true);
    ground->setParent(root);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    std::shared_ptr<Box> b = std::make_shared<Box>(Vec2(20, 20), (Vec2)sf::Mouse::getPosition(window));
                    b->setAnchored(true);
                    b->setParent(root);
                } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    std::shared_ptr<Ball> b = std::make_shared<Ball>(10.0f, (Vec2)sf::Mouse::getPosition(window));
                    b->setAnchored(false);
                    b->setParent(root);
                }
            }
        }

        // tick
        root->tick();

        // render everything
        window.clear(sf::Color::White);
        root->render(window);
        window.display();
    }

    return 0;
}
