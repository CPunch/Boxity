#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include "core/VObject.hpp"
#include "core/Root.hpp"
#include "services/PhysicsService.hpp"
#include "services/RenderService.hpp"
#include "types/Vec2.hpp"

class Entity: public VObject {
protected:
    PhysicsService *pSrvc = nullptr;
    b2Body *body = nullptr;
    std::shared_ptr<Vec2> position = std::make_shared<Vec2>(0, 0);
    float angle = 0;
    bool anchored = false;

    virtual void prerender();
    virtual void update();

    // custom events
    void onParentRemove();
    void onParentAdd();

    bool createBody();
    void updateFixture(b2FixtureDef*);

public:
    Entity();

    void setPosition(Vec2);
    void setAngle(float);
    void setAnchored(bool);

    std::shared_ptr<Vec2> getPosition();
    float getAngle();
    bool getAnchored();
    b2Body* getBody();

    void tick(uint64_t);
};