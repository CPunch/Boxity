#include "services/RenderService.hpp"
#include "objects/Ball.hpp"
#include "objects/Box.hpp"

#include <iostream>

RenderService::RenderService(ObjectPtr r): Service(r) {
    srvType = RENDERSRV;
    window = new sf::RenderWindow(sf::VideoMode(400, 500), "Boxity");
    //window->setFramerateLimit(60);

    std::cout << "RenderService Loaded!" << std::endl;
}

void RenderService::addRenderable(ObjectPtr o) {
    std::cout << "Object " << o.get() << " (" << o->getName() << ") being added to renderList!" << std::endl;
    assert(isRenderable(o));
    rndrList.insert(o);
}

void RenderService::removeRenderable(ObjectPtr o) {
    std::cout << "Object " << o.get() << " (" << o->getName() << ") being removed from renderList!" << std::endl;
    for (auto it = rndrList.begin(); it != rndrList.end(); it++) {
        if ((*it).get() == o.get()) {
            rndrList.erase(it);
            return;
        }
    }
}

void RenderService::render() {
    window->clear(sf::Color::White);

    // walk through our render list and render everything
    for (auto iter = rndrList.begin(); iter != rndrList.end(); iter++)
        castObjPtr(*iter, VObject)->render(window);

    window->display();
}

void RenderService::pollEvents() {
    sf::Event event;

    // this is temporary so i can test stuff until I add an event system
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window->close();
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                std::shared_ptr<Box> b = std::make_shared<Box>(Vec2(20, 20), Vec2(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y));
                b->setAnchored(true);
                b->setAngle(180);
                b->setParent(root);
            } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                std::shared_ptr<Ball> b = std::make_shared<Ball>(10.0f, Vec2(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y));
                b->setAnchored(false);
                b->setParent(root);
            }
        }
    }
}

bool RenderService::isOpen() {
    return window->isOpen();
}