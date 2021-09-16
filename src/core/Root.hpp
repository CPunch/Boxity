#pragma once

#include "core/Object.hpp"
#include "core/Service.hpp"

#define ROOTTICKTMER 1000/60
#define SCALE 30.0f
#define SFML2BOX2D(x) (x)/SCALE
#define BOX2D2SFML(x) (x)*SCALE
#define BOX2DANGLE(x) ((x)/b2_pi)*180
#define SFMLANGLE(x)  180/b2_pi*(x)

class Root : public Object {
    Service* loadedServices[MAXSRV];

    static void tick_task(void*, uint64_t, uint64_t);

public:
    Root();
    ~Root();

    void tick();
    void init(); // because shared_from_this hates constructors
    void serializeDoc(pugi::xml_document &doc);

    ObjectPtr getRoot();
    Service* getService(SRVICETYPE);
};