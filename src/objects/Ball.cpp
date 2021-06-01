#include "objects/Ball.hpp"

Ball::Ball(): Entity() {
    name = "Ball";

    shape.setFillColor(sf::Color::Black);
    setRadius(10.0f);
}

Ball::Ball(float radi, Vec2 pos): Ball() {
    position->set(pos);
    setRadius(radi);
}

// ==================================== [[ SETTERS ]] ====================================

void Ball::setRadius(float radi) {
    radius = radi;
    shape.setOrigin(radius, radius);
    update();
}

// ==================================== [[ GETTERS ]] ====================================

float Ball::getRadius() {
    return radius;
}

// ==================================== [[ MISC. ]] ====================================

void Ball::render(sf::RenderWindow *win) {
    win->draw(shape);
}

void Ball::prerender() {
    shape.setRadius(radius);
    shape.setPosition(position->getSVec());
    shape.setRotation(angle);
}

void Ball::update() {
    b2CircleShape b2Shape;
    b2Shape.m_radius = SFML2BOX2D(radius);

    b2FixtureDef fixDef;
    fixDef.shape = &b2Shape;

    updateFixture(&fixDef);

    // now update sfml shape
    prerender();
}