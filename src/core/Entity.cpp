#include "core/Entity.hpp"

Entity::Entity(World *w): Object(w) {
    w->addEntity(this);

    // setup box2d stuff
    createBody();
}

Entity::~Entity() {
    wrld->getWorld()->DestroyBody(body);
    wrld->removeEntity(this);
}

// ==================================== [[ SETTERS ]] ====================================

void Entity::setPosition(sf::Vector2f pos) {
    position = pos;
    update();
}

void Entity::setAngle(float a) {
    angle = a;
    update();
}

void Entity::setAnchored(bool a) {
    anchored = a;
    createBody();
    update();
}

// ==================================== [[ GETTERS ]] ====================================

Vec2 Entity::getPosition() {
    return position;
}

float Entity::getAngle() {
    return angle;
}

b2Body* Entity::getBody() {
    return body;
}

bool Entity::getAnchored() {
    return anchored;
}

// ==================================== [[ MISC. ]] ====================================

// this should be overwritten
void Entity::render(sf::RenderWindow&) {}

// this should be overwritten so the class can update it's sfml shape
void Entity::prerender() {}

// this should be overwritten so the class can update it's box2d body
void Entity::update() {}

void Entity::createBody() {
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

void Entity::updateFixture(b2FixtureDef* fixDef) {
    fixDef->density = 1.0f;
    fixDef->friction = 0.3f;

    createBody();

    body->SetTransform(b2Vec2(SFML2BOX2D(position.x), SFML2BOX2D(position.y)), angle);
    body->CreateFixture(fixDef);
}

void Entity::tick() {
    // update pos & angle from box2d
    angle = SFMLANGLE(body->GetAngle());
    position = Vec2(BOX2D2SFML(body->GetPosition().x), BOX2D2SFML(body->GetPosition().y));
    prerender();
}