#pragma once

#include <graphics.hpp>
#include <physics.hpp>

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
    virtual void serialize(pugi::xml_node &node);
    virtual void deserialize(pugi::xml_node &node);

    static void addBindings(lua_State*);
    virtual void pushLua(lua_State*);

    static void registerLuaGetters(lua_State*);
    static void registerLuaSetters(lua_State*);
    static void registerLuaMethods(lua_State*);
    static void registerLuaChild(lua_State*);
};