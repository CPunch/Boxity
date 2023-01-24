#pragma once

#include "core/Singleton.hpp"
#include "core/Object.hpp"
#include "core/Service.hpp"

#define ROOTTICKTMER 1000/60
#define SCALE 30.0f
#define SFML2BOX2D(x) (x)/SCALE
#define BOX2D2SFML(x) (x)*SCALE
#define BOX2DANGLE(x) ((x)*(b2_pi/180))
#define SFMLANGLE(x)  ((x)*(180/b2_pi))

class Root : public Object, public Singleton<Root> {
    std::unordered_set<Service*> services;

    static void tick_task(void*, uint64_t, uint64_t);

public:
    Root();
    ~Root();

    void preFrame();
    void init(); // because shared_from_this hates constructors
    void serializeDoc(pugi::xml_document &doc);
    void deserializeDoc(pugi::xml_document &doc);

    void registerService(Service*);
    void unregisterService(Service*);
    ObjectPtr getRoot();
};