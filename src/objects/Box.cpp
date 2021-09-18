#include "objects/Box.hpp"
#include "core/ObjectFactory.hpp"

#define LIBNAME "Box"

Box::Box(): Entity() {
    name = "Box";
    className = LIBNAME;
    classType = OBJ_BOX;

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

void Box::serialize(pugi::xml_node &node) {
    node.prepend_attribute("size").set_value(size->toString().c_str());

    Entity::serialize(node);
}

void Box::deserialize(pugi::xml_node &node) {
    pugi::xml_attribute attr;

    if (!((attr = node.attribute("size")).empty())) {
        Vec2 tmp;
        if (tmp.fromString(attr.value()))
            setSize(tmp);
    }

    Entity::deserialize(node);
}

// ==================================== [[ LUA ]] ====================================

ObjectPtr Box::createBox() {
    return std::make_shared<Box>();
}

void Box::pushLua(lua_State *L) {
    pushRawLua(L, LIBNAME);
}

// ==================================== [[ LUA GETTERS ]] ====================================

void registerLuaGetters(lua_State *L) {
    static const luaL_Reg getters[] {
        {NULL, NULL}
    };

    luaL_setfuncs(L, getters, 0);
}

// ==================================== [[ LUA SETTERS ]] ====================================

void registerLuaSetters(lua_State *L) {
    static const luaL_Reg setters[] {
        {NULL, NULL}
    };

    luaL_setfuncs(L, setters, 0);
}

// ==================================== [[ LUA METHODS ]] ====================================

void registerLuaMethods(lua_State *L) {
    static const luaL_Reg methods[] {
        {NULL, NULL}
    };

    luaL_setfuncs(L, methods, 0);
}

void registerLuaChild(lua_State *L) {
    Entity::registerLuaChild(L);
    lua_pushstring(L, LIBNAME);
    lua_pushboolean(L, true);
    lua_rawset(L, -3); // adds "Box" to the child table
}

void Box::addBindings(lua_State *L) {
    registerClass(L, registerLuaSetters, registerLuaGetters, registerLuaMethods, registerLuaChild, LIBNAME);
    ObjectFactory::addObjClass(OBJ_BOX, LIBNAME, createBox);
}