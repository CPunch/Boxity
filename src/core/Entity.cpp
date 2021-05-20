#include "core/Root.hpp"
#include "core/Entity.hpp"

Entity::Entity(Object *p): Object(p) {
    // set our object feature flags
    addFlag(typeFlags, RENDEROBJ);
    addFlag(typeFlags, PHYSICOBJ);

    // setup box2d stuff
    createBody();
}

Entity::~Entity() {
    
}

void Entity::onParentRemove() {
    b2World *wrld = getWorld();

    // sanity check
    if (wrld == nullptr)
        return;

    // if we already have a body, destroy it
    if (body != nullptr)
        wrld->DestroyBody(body);

    body = nullptr;
}

void Entity::onParentAdd() {
    createBody();
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

// this should be overwritten so the class can update it's sfml shape
void Entity::prerender() {}

// this should be overwritten so the class can update it's box2d body
void Entity::update() {}

bool Entity::createBody() {
    b2World *wrld = getWorld();

    // sanity check
    if (wrld == nullptr)
        return false;

    // if we already have a body, destroy it
    if (body != nullptr)
        wrld->DestroyBody(body);

    // create body defintion & set
    b2BodyDef myBodyDef;
    myBodyDef.type = anchored ? b2_staticBody : b2_dynamicBody;
    myBodyDef.position.Set(SFML2BOX2D(position.x), SFML2BOX2D(position.y));
    myBodyDef.angle = BOX2DANGLE(angle);

    body = wrld->CreateBody(&myBodyDef);
    return true;
}

void Entity::updateFixture(b2FixtureDef* fixDef) {
    fixDef->density = 1.0f;
    fixDef->friction = 0.3f;

    // if createBody fails, it means we don't have a world parent
    if (!createBody())
        return;

    body->SetTransform(b2Vec2(SFML2BOX2D(position.x), SFML2BOX2D(position.y)), angle);
    body->CreateFixture(fixDef);
}

void Entity::tick() {
    // sanity check
    if (body == nullptr)
        return;

    // update pos & angle from box2d
    angle = SFMLANGLE(body->GetAngle());
    position = Vec2(BOX2D2SFML(body->GetPosition().x), BOX2D2SFML(body->GetPosition().y));
    prerender();
}