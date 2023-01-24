#pragma once

#include "core/Service.hpp"
#include "core/Singleton.hpp"
#include "core/VObject.hpp"

#include <graphics.hpp>
#include <set>

class RenderService : public Service, public Singleton<RenderService> {
private:
    // this will automagically sort our render queue so everything is drawn in the proper order by z-index :)
    struct zIndxOrder {
        bool operator() (const ObjectPtr obj1, const ObjectPtr obj2) const {
            return castObjPtr(obj1, VObject)->getZIndex() > castObjPtr(obj2, VObject)->getZIndex();
        }
    };

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