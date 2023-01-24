#include "core/Root.hpp"
#include "core/VObject.hpp"
#include "services/RenderService.hpp"

#include <iostream>

VObject::VObject() {
    addFlag(typeFlags, RENDEROBJ);
}

void VObject::onParentRemove() {
    if (parent == nullptr)
        goto _passVOPREvnt;

    RenderService::getSingleton().removeRenderable(shared_from_this());

_passVOPREvnt:
    // pass the event down
    Object::onParentRemove();
}

void VObject::onParentAdd() {
    if (parent == nullptr) {
        goto _passVOPAEvnt;
    }

    RenderService::getSingleton().addRenderable(shared_from_this());

_passVOPAEvnt:
    // pass the event down
    Object::onParentAdd();
}

void VObject::serialize(pugi::xml_node &node) {
    node.prepend_attribute("zindex").set_value(zIndx);

    Object::serialize(node);
}

void VObject::deserialize(pugi::xml_node &node) {
    pugi::xml_attribute attr;

    if (!((attr = node.attribute("zindex")).empty()))
        zIndx = attr.as_int();

    Object::deserialize(node);
}

// ==================================== [[ GETTERS ]] ====================================

int VObject::getZIndex() {
    return zIndx;
}

// ==================================== [[ MISC. ]] ====================================

void VObject::render(sf::RenderWindow *win) {}