#include "Manager.hpp"

$on_mod(Loaded) {
    Manager::setHookEnabled("cocos2d::CCTransitionFade::create", false);
}

Manager& Manager::get() {
    static Manager instance;
    return instance;
}

bool Manager::isBrowserLayerAdded(LevelBrowserLayer* layer, int levelID) {
    auto& m = get();
    
    if (!m.browserLayersForID.contains(levelID)) {
        return false;
    }
    
    for (auto _layer : m.browserLayersForID.at(levelID)) {
        if (_layer == layer) {
            return true;
        }
    }
    
    return false;
}

void Manager::addBrowserLayer(LevelBrowserLayer* layer, int levelID) {
    if (isBrowserLayerAdded(layer, levelID)) {
        return;
    }
    
    auto& m = get();
    
    if (!m.browserLayersForID.contains(levelID)) {
        m.browserLayersForID[levelID] = std::vector<LevelBrowserLayer*>{};
    }
    
    m.browserLayersForID.at(levelID).push_back(layer);
}

void Manager::removeBrowserLayer(LevelBrowserLayer* layer) {
    auto& m = get();
    
    for (auto& [id, v] : m.browserLayersForID) {
        if (!isBrowserLayerAdded(layer, id)) {
            continue;
        }
        
        auto it = std::find(v.begin(), v.end(), layer);
        if (it != v.end()) {
            v.erase(it);
        }
    }
    
    if (m.currentLayer == layer) {
        m.currentLayer = nullptr;
    }
    
    if (m.currentLayer == layer) {
        m.currentLinkLayer = nullptr;
    }
    
    m.shouldLinkInfoLayer = false;
}

GJGameLevel* Manager::findNextLevel(CCArray* array, GJGameLevel* level) {
    if (!array) {
        return nullptr;
    }

    auto levels = CCArrayExt<GJGameLevel*>(array);
    auto index = level != nullptr ? -1 : 0;
    
    if (level) {
        for (int i = 0; i < levels.size(); i++) {
            if (EditorIDs::getID(level) == EditorIDs::getID(levels[i])) {
                index = i;
                break;
            }
        }
    }
    
    if (index <= -1) {
        return nullptr;
    }
    
    GJGameLevel* nextLevel = nullptr;
    
    for (int i = index; i < levels.size(); i++) {
        auto _level = levels[i];
        
        if (level && EditorIDs::getID(level) == EditorIDs::getID(_level)) {
            continue;
        }

        if (!GameStatsManager::get()->hasCompletedLevel(_level)) {
            nextLevel = _level;
            break;
        }
    }
    
    return nextLevel;
}

Ref<CCArray> Manager::getLevelsArray(LevelBrowserLayer* layer) {
    if (layer->m_levels && layer->m_levels->count() > 0) {
        return layer->m_levels;
    }

    auto array = CCArray::create();

    for (const auto& cell : layer->m_list->m_listView->m_tableView->m_contentLayer->getChildrenExt<LevelCell*>()) {
        array->addObject(cell->m_level);
    }

    if (array->count() <= 0) {
        return nullptr;
    }

    return array;
}

void Manager::setHookEnabled(const std::string& name, bool enabled) {
    for (Hook* hook : Mod::get()->getHooks()) {
        if (hook->getDisplayName() == name) {
            (void)(enabled ? hook->enable() : hook->disable());
            break;
        }
    }
}

bool Manager::isLevelInArray(CCArray* array, GJGameLevel* level) {
    for (const auto& _level : CCArrayExt<GJGameLevel*>(array)) {
        if (EditorIDs::getID(_level) == EditorIDs::getID(level)) {
            return true;
        }
    }

    return false;
}
