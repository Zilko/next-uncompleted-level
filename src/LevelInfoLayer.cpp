#include "LevelInfoLayer.hpp"

void ProLevelInfoLayer::onEnterTransitionDidFinish() {
    LevelInfoLayer::onEnterTransitionDidFinish();

    if (Manager::get().goingToLevel) {
        Loader::get()->queueInMainThread([self = Ref(this)] {
            self->onBack(nullptr);
        });
    }
}

bool ProLevelInfoLayer::init(GJGameLevel* level, bool challenge) {
    auto& m = Manager::get();

    if (!LevelInfoLayer::init(level, challenge)) {
        m.shouldLinkInfoLayer = false;
        m.currentLinkLayer = nullptr;
        return false;
    }
    
    if (m.shouldLinkInfoLayer) {
        m_fields->m_browserLayer = m.currentLinkLayer;
    }
    
    m.shouldLinkInfoLayer = false;
    m.currentLinkLayer = nullptr;
    
    return true;
}

void ProLevelInfoLayer::onPlay(CCObject* sender) {
    LevelInfoLayer::onPlay(sender);
    
    if (!m_isBusy || !m_enterTransitionFinished) {
        return;
    }
    
    auto f = m_fields.self();
    
    if (f->m_browserLayer) {
        Manager::get().currentLayer = f->m_browserLayer;
    }
}