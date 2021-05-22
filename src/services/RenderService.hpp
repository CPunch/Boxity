#pragma once

#include "core/Service.hpp"
#include "core/VObject.hpp"

#include <SFML/Graphics.hpp>
#include <set>

struct zIndxOrder {
    bool operator() (const ObjectPtr obj1, const ObjectPtr obj2) const {
        return ((VObject*)(obj1.get()))->getZIndex() > ((VObject*)(obj2.get()))->getZIndex();
    }
};

class RenderService : public Service {
private:
    sf::RenderWindow *window;
    std::multiset<ObjectPtr, zIndxOrder> rndrList; 

public:
    RenderService();

    // these ONLY ACCEPT VObject* !!!!!
    void addRenderable(ObjectPtr);
    void removeRenderable(ObjectPtr);

    void render();
    void pollEvents();
    bool isOpen();
};