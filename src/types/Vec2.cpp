#include "types/Vec2.hpp"
#include "core/Root.hpp"

#define LIBNAME "Vec2"

Vec2::Vec2(): x(0), y(0) {}
Vec2::Vec2(float _x, float _y): x(_x), y(_y) {}

// ==================================== [[ GETTERS ]] ====================================

sf::Vector2f Vec2::getSVec() {
    return sf::Vector2f(x, y);
}

b2Vec2 Vec2::getBVec() {
    return b2Vec2(SFML2BOX2D(x), SFML2BOX2D(y));
}

float Vec2::getX() {
    return x;
}

float Vec2::getY() {
    return y;
}

// ==================================== [[ SETTERS ]] ====================================

void Vec2::set(Vec2 other) {
    x = other.x;
    y = other.y;

    setUpdated(true);
}

void Vec2::setSVec(sf::Vector2f vec) {
    x = vec.x;
    y = vec.y;

    setUpdated(true);
}

void Vec2::setBVec(b2Vec2 vec) {
    x = BOX2D2SFML(vec.x);
    y = BOX2D2SFML(vec.y);

    setUpdated(true);
}

void Vec2::setX(float _x) {
    x = _x;

    setUpdated(true);
}

void Vec2::setY(float _y) {
    y = _y;

    setUpdated(true);
}

std::string Vec2::toString() {
    return std::to_string(x) + "," + std::to_string(y);
}

bool Vec2::fromString(std::string str) {
    size_t yStart = str.find(',') + 1;

    // if it wasn't found, abort!
    if (yStart == std::string::npos || yStart == str.size())
        return false;

    setX(strtof(str.c_str(), nullptr)); // should stop parsing whenever it hits ','
    setY(strtof(str.c_str() + yStart, nullptr));

    return true;
}

// ==================================== [[ LUA ]] ====================================

void Vec2::pushLua(lua_State *L) {
    pushRawLua(L, LIBNAME);
}

// ==================================== [[ LUA GETTERS ]] ====================================

static int vec2GetX(lua_State *L) {
    TypePtr *tPtr = Type::grabLua(L, 1, LIBNAME);

    if (tPtr != nullptr) {
        lua_pushnumber(L, castTypePtr(*tPtr, Vec2)->getX());
        return 1;
    }

    return 0;
}

static int vec2GetY(lua_State *L) {
    TypePtr *tPtr = Type::grabLua(L, 1, LIBNAME);

    if (tPtr != nullptr) {
        lua_pushnumber(L, castTypePtr(*tPtr, Vec2)->getY());
        return 1;
    }

    return 0;
}

void v2registerLuaGetters(lua_State *L) {
    static const luaL_Reg getters[] {
        {"x", vec2GetX},
        {"y", vec2GetY},
        {NULL, NULL}
    };

    Type::registerLuaGetters(L); // register parent class getters first
    luaL_setfuncs(L, getters, 0);
}

// ==================================== [[ LUA SETTERS ]] ====================================

static int vec2SetX(lua_State *L) {
    TypePtr *tPtr = Type::grabLua(L, 1, LIBNAME);
    if (tPtr == nullptr)
        return 0;

    lua_Number newX = luaL_checknumber(L, 2);
    castTypePtr(*tPtr, Vec2)->setX(newX);
    return 0;
}

static int vec2SetY(lua_State *L) {
    TypePtr *tPtr = Type::grabLua(L, 1, LIBNAME);
    if (tPtr == nullptr)
        return 0;

    lua_Number newX = luaL_checknumber(L, 2);
    castTypePtr(*tPtr, Vec2)->setY(newX);
    return 0;
}

void v2registerLuaSetters(lua_State *L) {
    static const luaL_Reg setters[] {
        {"x", vec2SetX},
        {"y", vec2SetY},
        {NULL, NULL}
    };

    Type::registerLuaSetters(L); // register parent class setters first
    luaL_setfuncs(L, setters, 0);
}

// ==================================== [[ LUA METHODS ]] ====================================

void v2registerLuaMethods(lua_State *L) {
    static const luaL_Reg methods[] {
        {NULL, NULL}
    };

    Type::registerLuaMethods(L); // register parent class methods first
    luaL_setfuncs(L, methods, 0);
}

static int vec2New(lua_State *L) {
    lua_Number x = luaL_optnumber(L, 1, 0);
    lua_Number y = luaL_optnumber(L, 2, 0);

    auto newVec = std::make_shared<Vec2>(x, y);
    newVec->pushLua(L);
    return 1;
}

static const luaL_Reg libMethods[] = {
    {"new", vec2New},
    {NULL, NULL}
};

void Vec2::addBindings(lua_State *L) {
    Type::registerClass(L, v2registerLuaSetters, v2registerLuaGetters, v2registerLuaMethods, LIBNAME);

    // register the constructor
    luaL_newlib(L, libMethods);
    lua_setglobal(L, "Vec2");
}