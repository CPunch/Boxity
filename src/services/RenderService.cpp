#include "services/RenderService.hpp"
#include "objects/Ball.hpp"
#include "objects/Box.hpp"

RenderService::RenderService() {
    srvType = RENDERSRV;
    window = new sf::RenderWindow(sf::VideoMode(400, 500), "SimpleNgi");
    window->setFramerateLimit(60);
}

void RenderService::addRenderable(ObjectPtr o) {
    //assert(isRenderable(o));
    rndrList.insert(o);
}

void RenderService::removeRenderable(ObjectPtr o) {
    rndrList.erase(o);
}

void RenderService::render() {
    window->clear(sf::Color::White);

    // walk through our render list and render everything
    for (auto iter = rndrList.begin(); iter != rndrList.end(); iter++)
        ((VObject*)(iter->get()))->render(window);

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
                std::shared_ptr<Box> b = std::make_shared<Box>(Vec2(20, 20), (Vec2)sf::Mouse::getPosition(*window));
                b->setAnchored(true);
                b->setParent(ObjectPtr(root));
            } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                std::shared_ptr<Ball> b = std::make_shared<Ball>(10.0f, (Vec2)sf::Mouse::getPosition(*window));
                b->setAnchored(false);
                b->setParent(ObjectPtr(root));
            }
        }
    }
}

bool RenderService::isOpen() {
    return window->isOpen();
}