#include "core/Root.hpp"
#include "core/Entity.hpp"

#define LIBNAME "Entity"

Entity::Entity(): VObject() {
    name = "Entity";
    className = LIBNAME;

    // set our object feature flags
    addFlag(typeFlags, ENTITYOBJ);
}

void Entity::onParentRemove() {
    b2World *wrld;

    // sanity check
    if (pSrvc == nullptr)
        goto _rmvBody;

    wrld = pSrvc->getWorld();

    // if we already have a body, destroy it
    if (body != nullptr)
        wrld->DestroyBody(body);

_rmvBody:
    body = nullptr;

    // pass the event down
    VObject::onParentRemove();
}

void Entity::onParentAdd() {
    // sanity check
    if (root == nullptr) {
        pSrvc = nullptr;
        goto _passEOPAEvnt;
    }

    // grab the physics service & then create our new body
    pSrvc = (PhysicsService*)castObjPtr(root, Root)->getService(PHYSICSRV);
    update();

_passEOPAEvnt:
    // pass the event down
    VObject::onParentAdd();
}

void Entity::serialize(pugi::xml_node &node) {
    node.prepend_attribute("angle").set_value(angle);
    node.prepend_attribute("anchored").set_value(anchored);

    node.append_attribute("position").set_value(position->toString().c_str());

    // serialize the base class attributes
    VObject::serialize(node);
}

void Entity::deserialize(pugi::xml_node &node) {
    pugi::xml_attribute attr;

    if (!((attr = node.attribute("angle")).empty()))
        angle = attr.as_float();

    if (!((attr = node.attribute("anchored")).empty()))
        anchored = attr.as_bool();

    if (!((attr = node.attribute("position")).empty()))
        position->fromString(attr.value());

    // deserialize the base class attributes
    VObject::deserialize(node);
}

// ==================================== [[ SETTERS ]] ====================================

void Entity::setPosition(Vec2 pos) {
    position->set(pos);
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

std::shared_ptr<Vec2> Entity::getPosition() {
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
    // sanity check
    if (pSrvc == nullptr)
        return false;

    b2World *wrld = pSrvc->getWorld();

    // if we already have a body, destroy it
    if (body != nullptr)
        wrld->DestroyBody(body);

    // create body defintion & set
    b2BodyDef myBodyDef;
    myBodyDef.type = anchored ? b2_staticBody : b2_dynamicBody;
    myBodyDef.position = position->getBVec();
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

    body->SetTransform(position->getBVec(), angle);
    body->CreateFixture(fixDef);
}

void Entity::tick(uint64_t t) {
    // sanity check
    if (body == nullptr)
        return;

    angle = SFMLANGLE(body->GetAngle());

    if (!position->getUpdated()) {
        // update pos from box2d
        position->setBVec(body->GetPosition());
    } else {
        // update(), which calls updateFixture which updates the box2d position
        update();
    }

    position->setUpdated(false);

    prerender();

    Object::tick(t);
}

// ==================================== [[ LUA ]] ====================================

void Entity::pushLua(lua_State *L) {
    pushRawLua(L, LIBNAME);
}

// ==================================== [[ LUA GETTERS ]] ====================================

static int luaGetPosition(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, LIBNAME);

    if (oPtr == nullptr)
        return 0;

    // push position & return
    castObjPtr(*oPtr, Entity)->getPosition()->pushLua(L);
    return 1;
}

static int luaGetAngle(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, LIBNAME);

    if (oPtr == nullptr)
        return 0;

    // push angle & return
    lua_pushnumber(L, castObjPtr(*oPtr, Entity)->getAngle());
    return 1;
}

static int luaGetAnchored(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, LIBNAME);

    if (oPtr == nullptr)
        return 0;

    // push anchored & return
    lua_pushboolean(L, castObjPtr(*oPtr, Entity)->getAnchored());
    return 1;
}

void Entity::registerLuaGetters(lua_State *L) {
    static const luaL_Reg getters[] {
        {"position", luaGetPosition},
        {"angle", luaGetAngle},
        {"anchored", luaGetAnchored},
        {NULL, NULL}
    };

    Object::registerLuaGetters(L);
    luaL_setfuncs(L, getters, 0);
}

// ==================================== [[ LUA SETTERS ]] ====================================

static int luaSetPosition(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, LIBNAME);
    TypePtr *tPtr = Type::grabLua(L, 2, "Vec2");

    if (oPtr != nullptr && tPtr != nullptr)
        castObjPtr(*oPtr, Entity)->setPosition(*(castTypePtr(*tPtr, Vec2)));

    return 0;
}

static int luaSetAngle(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, LIBNAME);
    lua_Number newAngle = luaL_checknumber(L, 2);

    if (oPtr != nullptr)
        castObjPtr(*oPtr, Entity)->setAngle(newAngle);

    return 0;
}

static int luaSetAnchored(lua_State *L) {
    ObjectPtr *oPtr = Object::grabLua(L, 1, LIBNAME);
    bool newAnchor = lua_toboolean(L, 2);

    if (oPtr != nullptr)
        castObjPtr(*oPtr, Entity)->setAnchored(newAnchor);

    return 0;
}

void Entity::registerLuaSetters(lua_State *L) {
    static const luaL_Reg setters[] {
        {"position", luaSetPosition},
        {"angle", luaSetAngle},
        {"anchored", luaSetAnchored},
        {NULL, NULL}
    };

    Object::registerLuaSetters(L);
    luaL_setfuncs(L, setters, 0);
}

// ==================================== [[ LUA METHODS ]] ====================================

void Entity::registerLuaMethods(lua_State *L) {
    static const luaL_Reg methods[] {
        {NULL, NULL}
    };

    Object::registerLuaMethods(L);
    luaL_setfuncs(L, methods, 0);
}

void Entity::registerLuaChild(lua_State *L) {
    Object::registerLuaChild(L);
    lua_pushstring(L, LIBNAME);
    lua_pushboolean(L, true);
    lua_rawset(L, -3); // adds "Entity" to the child table
}

void Entity::addBindings(lua_State *L) {
    registerClass(L, registerLuaSetters, registerLuaGetters, registerLuaMethods, registerLuaChild, LIBNAME);
}