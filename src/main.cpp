#include "core/Root.hpp"

#include "services/RenderService.hpp"
#include "objects/Box.hpp"
#include "objects/Ball.hpp"
#include "objects/Script.hpp"

#include "pugixml.hpp"

#include <iostream>

int main() {
    std::shared_ptr<Root> root = std::make_shared<Root>();
    root->init();

    RenderService *rSrvc = (RenderService*)root->getService(RENDERSRV);
    root->getService(SCRIPTSRV); // load script service since the deserializer depends on it

    pugi::xml_document doc;
    doc.load_file("examples/ballstack.bx");
    root->deserializeDoc(doc);

    while (rSrvc->isOpen()) {
        rSrvc->pollEvents();

        // tick
        root->tick();

        // render everything
        rSrvc->render();
    }

    root->remove();
    std::cout << "root removed!" << std::endl;
    return 0;
}
