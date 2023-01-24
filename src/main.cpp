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

    pugi::xml_document doc;
    doc.load_file("examples/ballstack.bx");
    root->deserializeDoc(doc);

    while (RenderService::getSingleton().isOpen()) {
        RenderService::getSingleton().pollEvents();

        // tick
        root->preFrame();

        // render everything
        RenderService::getSingleton().render();
    }

    root->remove();
    std::cout << "root removed!" << std::endl;
    return 0;
}
