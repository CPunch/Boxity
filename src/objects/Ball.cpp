#include "objects/Ball.hpp"
#include "core/ObjectFactory.hpp"

#define LIBNAME "Ball"

Ball::Ball(): Entity() {
    name = "Ball";
    className = LIBNAME;
    classType = OBJ_BALL;

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

// ==================================== [[ LUA ]] ====================================

ObjectPtr Ball::createBall() {
    return std::make_shared<Ball>();
}

void Ball::pushLua(lua_State *L) {
    pushRawLua(L, LIBNAME);
}

// ==================================== [[ LUA GETTERS ]] ====================================

static int ballGetRadius(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, LIBNAME);

    if (oPtr == nullptr)
        return 0;

    lua_pushnumber(L, castObjPtr(*oPtr, Ball)->getRadius());
    return 1;
}

void registerBallGetters(lua_State *L) {
    static const luaL_Reg getters[] {
        {"radius", ballGetRadius},
        {NULL, NULL}
    };

    Entity::registerLuaGetters(L);
    luaL_setfuncs(L, getters, 0);
}

// ==================================== [[ LUA SETTERS ]] ====================================

static int ballSetRadius(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, LIBNAME);
    lua_Number newRadius = luaL_checknumber(L, 2);

    if (oPtr == nullptr)
        return 0;

    castObjPtr(*oPtr, Ball)->setRadius(newRadius);
    return 0;
}

void registerBallSetters(lua_State *L) {
    static const luaL_Reg setters[] {
        {"radius", ballSetRadius},
        {NULL, NULL}
    };

    Entity::registerLuaSetters(L);
    luaL_setfuncs(L, setters, 0);
}

// ==================================== [[ LUA METHODS ]] ====================================

void registerBallMethods(lua_State *L) {
    static const luaL_Reg methods[] {
        {NULL, NULL}
    };

    Entity::registerLuaMethods(L);
    luaL_setfuncs(L, methods, 0);
}

void registerBallChild(lua_State *L) {
    Entity::registerLuaChild(L);
    lua_pushstring(L, LIBNAME);
    lua_pushboolean(L, true);
    lua_rawset(L, -3); // adds "Ball" to the child table
}

void Ball::addBindings(lua_State *L) {
    registerClass(L, registerBallSetters, registerBallGetters, registerBallMethods, registerBallChild, LIBNAME);
    ObjectFactory::addObjClass(OBJ_BALL, LIBNAME, createBall);
}