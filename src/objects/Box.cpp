#include "objects/Box.hpp"

Box::Box(): Entity() {
    name = "Box";

    shape.setFillColor(sf::Color::Black);
}

Box::Box(Vec2 sz, Vec2 pos): Box() {
    position->set(pos);
    setSize(sz);
}

// ==================================== [[ SETTERS ]] ====================================

void Box::setSize(Vec2 sz) {
    size->set(sz);
    shape.setOrigin(sz.getX()/2, sz.getY()/2);
    update();
}

// ==================================== [[ GETTERS ]] ====================================

std::shared_ptr<Vec2> Box::getSize() {
    return size;
}

// ==================================== [[ MISC. ]] ====================================

void Box::prerender() {
    shape.setSize(size->getSVec());
    shape.setPosition(position->getSVec());
    shape.setRotation(angle);
}

void Box::update() {
    b2PolygonShape b2Shape;
    b2Shape.SetAsBox(size->getBVec().x/2, size->getBVec().y/2);

    b2FixtureDef fixDef;
    fixDef.shape = &b2Shape;

    updateFixture(&fixDef);

    // now update sfml shape
    prerender();
}

void Box::render(sf::RenderWindow *win) {
    win->draw(shape);
}

void Box::tick(uint64_t t) {
    if (size->getUpdated()) {
        update();
        size->setUpdated(false);
    }

    Entity::tick(t);
}