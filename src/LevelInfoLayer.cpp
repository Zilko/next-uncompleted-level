#include "LevelInfoLayer.hpp"

void ProLevelInfoLayer::levelDownloadFinished(GJGameLevel* p0) {
    LevelInfoLayer::levelDownloadFinished(p0);

    auto f = m_fields.self();

    if (f->m_shouldAutoOpen) {
        onPlay(nullptr);
    }
}

void ProLevelInfoLayer::levelDownloadFailed(int p0) {
    LevelInfoLayer::levelDownloadFailed(p0);
    m_fields->m_shouldAutoOpen = false;
}

void ProLevelInfoLayer::onEnterTransitionDidFinish() {
    LevelInfoLayer::onEnterTransitionDidFinish();

    auto& m = Manager::get();
    
    if (m.goingToLevel) {
        Loader::get()->queueInMainThread([self = Ref(this)] {
            self->onBack(nullptr);
        });
    }

    if (m_fields->m_shouldAutoOpen) {
        Loader::get()->queueInMainThread([self = Ref(this)] {
            if (!self->m_level->m_levelNotDownloaded) {
                self->onPlay(nullptr);
            }
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

    if (m.shouldAutoOpen) {
        m_fields->m_shouldAutoOpen = true;
        log::debug("wa");
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
    
    f->m_shouldAutoOpen = false;

    if (f->m_browserLayer) {
        Manager::get().currentLayer = f->m_browserLayer;
    }
}