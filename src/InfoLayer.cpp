#include "InfoLayer.hpp"
#include "NextButton.hpp"

bool ProInfoLayer::init(GJGameLevel* p0, GJUserScore* p1, GJLevelList* p2) {
    if (!InfoLayer::init(p0, p1, p2)) {
        return false;
    }

    auto menu = m_mainLayer->getChildByID("refresh-menu");

    if (!menu) {
        return false;
    }

    auto btn = NextButton::create([](Button*) {

    });
    btn->setScalePro(0.6f);

    menu->addChild(btn);
    menu->updateLayout();

    return true;
}