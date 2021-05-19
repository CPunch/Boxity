#include "core/PVObject.hpp"

PVObject::PVObject(World *w): Object(w) {
    w->addPVObject(this);

    // setup box2d stuff
    createBody();
}

PVObject::~PVObject() {
    wrld->getWorld()->DestroyBody(body);
    wrld->removePVObject(this);
}

// ==================================== [[ SETTERS ]] ====================================

void PVObject::setPosition(sf::Vector2f pos) {
    position = pos;
    update();
}

void PVObject::setAngle(float a) {
    angle = a;
    update();
}

void PVObject::setAnchored(bool a) {
    anchored = a;
    createBody();
    update();
}

// ==================================== [[ GETTERS ]] ====================================

Vec2 PVObject::getPosition() {
    return position;
}

float PVObject::getAngle() {
    return angle;
}

b2Body* PVObject::getBody() {
    return body;
}

bool PVObject::getAnchored() {
    return anchored;
}

// ==================================== [[ MISC. ]] ====================================

// this should be overwritten
void PVObject::render(sf::RenderWindow&) {}

// this should be overwritten so the class can update it's sfml shape
void PVObject::prerender() {}

// this should be overwritten so the class can update it's box2d body
void PVObject::update() {}

void PVObject::createBody() {
    // if we already have a body, destroy it
    if (body != nullptr)
        wrld->getWorld()->DestroyBody(body);

    // create body defintion & set
    b2BodyDef myBodyDef;
    myBodyDef.type = anchored ? b2_staticBody : b2_dynamicBody;
    myBodyDef.position.Set(SFML2BOX2D(position.x), SFML2BOX2D(position.y));
    myBodyDef.angle = BOX2DANGLE(angle);

    body = wrld->getWorld()->CreateBody(&myBodyDef);
}

void PVObject::updateFixture(b2FixtureDef* fixDef) {
    fixDef->density = 1.0f;
    fixDef->friction = 0.3f;

    createBody();

    body->SetTransform(b2Vec2(SFML2BOX2D(position.x), SFML2BOX2D(position.y)), angle);
    body->CreateFixture(fixDef);
}

void PVObject::tick() {
    // update pos & angle from box2d
    angle = SFMLANGLE(body->GetAngle());
    position = Vec2(BOX2D2SFML(body->GetPosition().x), BOX2D2SFML(body->GetPosition().y));
    prerender();
}