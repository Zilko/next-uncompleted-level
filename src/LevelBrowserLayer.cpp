#include "LevelBrowserLayer.hpp"
#include "EndLevelLayer.hpp"

ProLevelBrowserLayer::Fields::~Fields() {
    if (!m_self) {
        return;
    }
    
    Manager::removeBrowserLayer(m_self);
}

void ProLevelBrowserLayer::onEnterTransitionDidFinish() {
    LevelBrowserLayer::onEnterTransitionDidFinish();

    if (!Manager::get().goingToLevel) {
        return;
    }

    Loader::get()->queueInMainThread([self = Ref(this)] {
        int att = 0;

        while (!Manager::isLevelInArray(Manager::getLevelsArray(self), Manager::get().goingToLevel) && att < 10) {
            self->onNextPage(nullptr);
            att++;
        }

        GameLevelManager::get()->gotoLevelPage(Manager::get().goingToLevel);
        Manager::get().goingToLevel = nullptr;
        Manager::setHookEnabled("cocos2d::CCTransitionFade::create", false);
    });
}

void ProLevelBrowserLayer::loadLevelsFinished(CCArray* p0, const char* p1, int p2) {
    LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);
    
    auto f = m_fields.self();
    
    if (f->m_allowedAttempts <= 0 || !f->m_endLayer) {
        return;
    }
    
    f->m_allowedAttempts--;
    
    auto nextLevel = Manager::findNextLevel(m_levels, nullptr);

    if (!nextLevel && f->m_allowedAttempts > 0) {
        auto prevLevels = m_levels;
        
        while (true) {
            onNextPage(nullptr);
            
            f->m_skippedPages++;
            
            if (prevLevels != m_levels) {
                if (!m_levels || m_levels->count() <= 0) {
                    f->m_allowedAttempts = 0;

                    for (int i = 0; i < f->m_skippedPages; i++) {
                        onPrevPage(nullptr);
                    }

                    f->m_skippedPages = 0;
                    f->m_endLayer = nullptr;
                    f->m_currentLevel = nullptr;
                    
                    Notification::create("Reached end of pages.")->show();

                    return;
                }
                
                nextLevel = Manager::findNextLevel(m_levels, nullptr);
            
                if (!nextLevel) {
                    prevLevels = m_levels;
                    continue;
                }
                
                f->m_allowedAttempts = 0;

                for (int i = 0; i < f->m_skippedPages; i++) {
                    onPrevPage(nullptr);
                }

                auto endLayer = static_cast<ProEndLevelLayer*>(f->m_endLayer);
                endLayer->m_fields->m_nextLevel = nextLevel;
                endLayer->setButtonEnabled(true);

                f->m_skippedPages = 0;
                f->m_endLayer = nullptr;
                f->m_currentLevel = nullptr;
                
                return;
            }
            
            break;
        }
    } else {
        f->m_allowedAttempts = 0;

        for (int i = 0; i < f->m_skippedPages; i++) {
            onPrevPage(nullptr);
        }

        if (!f->m_endLayer) {
            return;
        }

        auto endLayer = static_cast<ProEndLevelLayer*>(f->m_endLayer);
        
        if (nextLevel) {
            endLayer->m_fields->m_nextLevel = nextLevel;
            endLayer->setButtonEnabled(true);
        } else {
            endLayer->setButtonEnabled(false);
            Notification::create("Couldn't find uncompleted level within 3 pages.")->show();
        }

        f->m_skippedPages = 0;
        f->m_endLayer = nullptr;
        f->m_currentLevel = nullptr;
    }
}

void ProLevelBrowserLayer::loadLevelsFailed(const char* p0, int p1) {
    LevelBrowserLayer::loadLevelsFailed(p0, p1);
    
    auto f = m_fields.self();

    for (int i = 0; i < f->m_skippedPages; i++) {
        onPrevPage(nullptr);
    }
    
    f->m_skippedPages = 0;
    f->m_allowedAttempts = 0;
    f->m_endLayer = nullptr;
    f->m_currentLevel = nullptr;
}