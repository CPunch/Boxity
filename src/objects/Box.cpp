#include "objects/Box.hpp"

Box::Box(): Entity() {
    shape.setFillColor(sf::Color::Black);
}

Box::Box(Vec2 sz, Vec2 pos): Box() {
    position = pos;
    setSize(sz);
}

// ==================================== [[ SETTERS ]] ====================================

void Box::setSize(Vec2 sz) {
    size = sz;
    shape.setOrigin(sz.x/2, sz.y/2);
    update();
}

// ==================================== [[ GETTERS ]] ====================================

Vec2 Box::getSize() {
    return size;
}

// ==================================== [[ MISC. ]] ====================================

void Box::prerender() {
    shape.setSize(size);
    shape.setPosition(position);
    shape.setRotation(angle);
}

void Box::update() {
    b2PolygonShape b2Shape;
    b2Shape.SetAsBox(SFML2BOX2D(size.x/2), SFML2BOX2D(size.y/2));

    b2FixtureDef fixDef;
    fixDef.shape = &b2Shape;

    updateFixture(&fixDef);

    // now update sfml shape
    prerender();
}

void Box::render(sf::RenderWindow *win) {
    win->draw(shape);
}