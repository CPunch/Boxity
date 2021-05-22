#pragma once

#include "core/Service.hpp"
#include "core/VObject.hpp"

#include <SFML/Graphics.hpp>
#include <set>

struct zIndxOrder {
    bool operator() (const ObjectPtr obj1, const ObjectPtr obj2) const {
        return castObjPtr(obj1, VObject)->getZIndex() > castObjPtr(obj2, VObject)->getZIndex();
    }
};

class RenderService : public Service {
private:
    std::multiset<ObjectPtr, zIndxOrder> rndrList; 

public:
    sf::RenderWindow *window;
    RenderService();

    // these ONLY ACCEPT VObject* !!!!!
    void addRenderable(ObjectPtr);
    void removeRenderable(ObjectPtr);

    void render();
    void pollEvents();
    bool isOpen();
};