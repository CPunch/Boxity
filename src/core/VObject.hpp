#pragma once

#include <graphics.hpp>
#include "core/Object.hpp"

#include <memory>

class RenderService;

// visible object
class VObject : public Object {
protected:
    RenderService *rSrvc = nullptr;
    int zIndx = -1;

    virtual void onParentRemove();
    virtual void onParentAdd();

public:
    VObject();

    virtual int getZIndex();

    virtual void serialize(pugi::xml_node &node);
    virtual void render(sf::RenderWindow*);
};