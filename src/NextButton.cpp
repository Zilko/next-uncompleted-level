#include "NextButton.hpp"

NextButton* NextButton::create(ButtonCallback activateCallback) {
    auto ret = new NextButton();

    ret->myInit(std::move(activateCallback));
    ret->autorelease();

    return ret;
}

void NextButton::setEnabled(bool enabled) {
    Button::setEnabled(enabled);

    m_spr1->setOpacity(enabled ? 255 : 0);
    m_spr2->setVisible(!enabled);
    m_spr3->setVisible(!enabled);

    if (m_loadingCircle) {
        m_loadingCircle->removeFromParent();
        m_loadingCircle = nullptr;
    }
}

void NextButton::addLoadingCircle() {
    if (m_loadingCircle) {
        m_loadingCircle->removeFromParent();
        m_loadingCircle = nullptr;
    }

    m_loadingCircle = CCSprite::create("loadingCircle.png");
    m_loadingCircle->setPosition(getContentSize() / 2.f);
    m_loadingCircle->setOpacity(138);
    m_loadingCircle->setScale(0.425f);
    m_loadingCircle->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)));

    addChild(m_loadingCircle);
}

void NextButton::setScalePro(float scale) {
    setContentSize(getContentSize() * scale);
    getDisplayNode()->setScale(scale);
    getDisplayNode()->setPosition(getContentSize() / 2.f);
    m_loadingCircle->setPosition(getContentSize() / 2.f);
    m_loadingCircle->setScale(0.425f * scale);
}

bool NextButton::myInit(ButtonCallback activateCallback) {
    auto spr = CCSprite::createWithSpriteFrameName("GJ_plainBtn_001.png");

    m_spr2 = CCSpriteGrayscale::createWithSpriteFrameName("GJ_plainBtn_001.png");
    m_spr2->setAnchorPoint({0, 0});
    m_spr2->setColor({85, 85, 85});

    spr->addChild(m_spr2);

    m_spr1 = CCSprite::create("arrows.png"_spr);
    m_spr1->setPosition(spr->getContentSize() / 2.f + ccp(2, -1));

    spr->addChild(m_spr1);

    m_spr3 = CCSpriteGrayscale::create("arrows.png"_spr);
    m_spr3->setAnchorPoint({0, 0});
    m_spr3->setColor({85, 85, 85});
    m_spr3->setOpacity(141);

    m_spr1->addChild(m_spr3);

    Button::initWithNode(spr, std::move(activateCallback));

    addLoadingCircle();

    return true;
}