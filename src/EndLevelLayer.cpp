#include "EndLevelLayer.hpp"
#include "LevelBrowserLayer.hpp"
#include "NextButton.hpp"

void ProEndLevelLayer::onNextLevel() {
    auto f = m_fields.self();

    if (!f->m_nextLevel) {
        return;
    }

    Manager::get().goingToLevel = f->m_nextLevel;
    Manager::get().transitionCount = 0;
    Manager::setHookEnabled("cocos2d::CCTransitionFade::create", true);

    onMenu(nullptr);
}

void ProEndLevelLayer::setButtonEnabled(bool enabled) {
    auto f = m_fields.self();

    if (f->m_button) {
        f->m_button->setEnabled(enabled);
    }
}

void ProEndLevelLayer::customSetup() {
    EndLevelLayer::customSetup();

    auto& m = Manager::get();

    if (!m.currentLayer) {
        return;
    }

    if (Mod::get()->getSettingValue<bool>("disable")) {
        return;
    }
    
    auto menu = m_mainLayer->getChildByID("button-menu");

    if (!menu) {
        return;
    }

    auto levels = Manager::getLevelsArray(m.currentLayer);
    auto nextLevel = Manager::findNextLevel(levels, m_playLayer->m_level);
    auto f = m_fields.self();

    f->m_button = NextButton::create([this](Button*) {
        onNextLevel();
    });
    f->m_button->setPosition(180, -125);
    f->m_button->setID("next-level-btn"_spr);

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

        f->m_button->addLoadingCircle();

        break;
    }
    
}