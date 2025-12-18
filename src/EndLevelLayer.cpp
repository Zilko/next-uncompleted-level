#include "EndLevelLayer.hpp"
#include "LevelBrowserLayer.hpp"

void ProEndLevelLayer::onNextLevel(CCObject*) {
    auto f = m_fields.self();

    if (!f->m_nextLevel) {
        return;
    }

    Manager::get().goingToLevel = f->m_nextLevel;
    Manager::setHookEnabled("cocos2d::CCTransitionFade::create", true);

    onMenu(nullptr);
}

void ProEndLevelLayer::setButtonEnabled(bool enabled) {
    auto f = m_fields.self();

    if (!f->m_button) {
        return;
    }

    f->m_button->setEnabled(enabled);
    f->m_spr1->setOpacity(enabled ? 255 : 0);
    f->m_spr2->setVisible(!enabled);
    f->m_spr3->setVisible(!enabled);
    
    if (f->m_loadingCircle) {
        f->m_loadingCircle->removeFromParentAndCleanup(true);
        f->m_loadingCircle = nullptr;
    }
}

void ProEndLevelLayer::customSetup() {
    EndLevelLayer::customSetup();

    auto& m = Manager::get();

    if (!m.currentLayer) {
        return;
    }
    
    auto levels = Manager::getLevelsArray(m.currentLayer);
    auto nextLevel = Manager::findNextLevel(levels, m_playLayer->m_level);
    auto f = m_fields.self();

    auto spr = CCSprite::createWithSpriteFrameName("GJ_plainBtn_001.png");

    f->m_spr2 = CCSpriteGrayscale::createWithSpriteFrameName("GJ_plainBtn_001.png");
    f->m_spr2->setAnchorPoint({0, 0});
    f->m_spr2->setColor({85, 85, 85});

    spr->addChild(f->m_spr2);

    f->m_spr1 = CCSprite::create("arrows.png"_spr);
    f->m_spr1->setPosition(spr->getContentSize() / 2.f + ccp(2, -1));

    spr->addChild(f->m_spr1);

    f->m_spr3 = CCSpriteGrayscale::create("arrows.png"_spr);
    f->m_spr3->setAnchorPoint({0, 0});
    f->m_spr3->setColor({85, 85, 85});
    f->m_spr3->setOpacity(141);

    f->m_spr1->addChild(f->m_spr3);

    f->m_button = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ProEndLevelLayer::onNextLevel));
    f->m_button->setPosition(180, -125);
    f->m_button->setID("next-level-btn"_spr);

    auto menu = m_mainLayer->getChildByID("button-menu");

    menu->addChild(f->m_button);
    
    setButtonEnabled(false);

    if (nextLevel) {
        f->m_nextLevel = nextLevel;
        setButtonEnabled(true);
        return;
    }
    
    int skippedPages = 0;
    
    while (true) {
        m.currentLayer->onNextPage(nullptr);
        
        skippedPages++;
        
        if (levels != m.currentLayer->m_levels) {
            if (!m.currentLayer->m_levels || m.currentLayer->m_levels->count() <= 0) {
                for (int i = 0; i < skippedPages; i++) {
                    m.currentLayer->onPrevPage(nullptr);
                }
                
                return;
            }
            
            nextLevel = Manager::findNextLevel(m.currentLayer->m_levels, nullptr);
        
            if (!nextLevel) {
                levels = m.currentLayer->m_levels;
                continue;
            }
            
            for (int i = 0; i < skippedPages; i++) {
                m.currentLayer->onPrevPage(nullptr);
            }
            
            f->m_nextLevel = nextLevel;
            setButtonEnabled(true);

            return;
        }

        if (typeinfo_cast<LevelListLayer*>(m.currentLayer)) {
            return;
        }
        
        auto ff = static_cast<ProLevelBrowserLayer*>(m.currentLayer)->m_fields.self();
        
        ff->m_endLayer = this;
        ff->m_skippedPages = skippedPages;
        ff->m_allowedAttempts = 3;
        ff->m_currentLevel = m_playLayer->m_level;

        f->m_loadingCircle = CCSprite::create("loadingCircle.png");
        f->m_loadingCircle->setPosition(spr->getContentSize() / 2.f);
        f->m_loadingCircle->setOpacity(138);
        f->m_loadingCircle->setScale(0.425f);
        f->m_loadingCircle->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)));

        f->m_button->addChild(f->m_loadingCircle);

        break;
    }
    
}