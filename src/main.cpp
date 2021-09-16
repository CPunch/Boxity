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

    std::shared_ptr<Box> ground = std::make_shared<Box>(Vec2(400, 50), Vec2(200, 400));
    ground->setAnchored(true);
    ground->setParent(root);

    std::shared_ptr<Script> scrpt = std::make_shared<Script>();
    scrpt->setParent(root);
    scrpt->setSource("while true do wait(1) print('making a ball!!!') ball = Factory.new(\"Ball\") ball.position = Vec2.new(200, 100) ball.radius = 10 ball.parent = root end");

    pugi::xml_document doc;
    root->serializeDoc(doc);
    doc.save(std::cout);

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
