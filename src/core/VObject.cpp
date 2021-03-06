#include "core/Root.hpp"
#include "core/VObject.hpp"
#include "services/RenderService.hpp"

#include <iostream>

VObject::VObject() {
    addFlag(typeFlags, RENDEROBJ);
}

void VObject::onParentRemove() {
    if (rSrvc == nullptr)
        goto _passVOPREvnt;

    rSrvc->removeRenderable(shared_from_this());
    rSrvc = nullptr;

_passVOPREvnt:
    // pass the event down
    Object::onParentRemove();
}

void VObject::onParentAdd() {
    if (root == nullptr) {
        rSrvc = nullptr;
        goto _passVOPAEvnt;
    }

    // grab the render service
    rSrvc = (RenderService*)castObjPtr(root, Root)->getService(RENDERSRV);

    if (rSrvc == nullptr) {
        goto _passVOPAEvnt;
    }

    rSrvc->addRenderable(shared_from_this());

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