#include "LevelCell.hpp"
#include "LevelBrowserLayer.hpp"

void ProLevelCell::onClick(CCObject* sender) {
    auto scene = CCScene::get();
    
    if (auto node = scene->getChildByType<LevelBrowserLayer>(0)) {
        auto browserLayer = static_cast<ProLevelBrowserLayer*>(node);
        
        browserLayer->m_fields->m_self = browserLayer;
        
        auto& m = Manager::get();
        
        m.shouldLinkInfoLayer = true;
        m.currentLinkLayer = browserLayer;
        
        Manager::addBrowserLayer(browserLayer, EditorIDs::getID(m_level));
    }
    
    LevelCell::onClick(sender);
}