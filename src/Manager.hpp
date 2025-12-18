#pragma once

#include "Includes.hpp"

class Manager {
  
public:

    LevelBrowserLayer* currentLayer = nullptr;
    LevelBrowserLayer* currentLinkLayer = nullptr;

    GJGameLevel* goingToLevel = nullptr;

    std::unordered_map<int, std::vector<LevelBrowserLayer*>> browserLayersForID;
    
    bool shouldLinkInfoLayer = false;

    int skippedPages = 0;

    static Manager& get();
    
    static bool isBrowserLayerAdded(LevelBrowserLayer*, int);
    
    static void addBrowserLayer(LevelBrowserLayer*, int);
    
    static void removeBrowserLayer(LevelBrowserLayer*);
    
    static GJGameLevel* findNextLevel(CCArray*, GJGameLevel*);

    static Ref<CCArray> getLevelsArray(LevelBrowserLayer*);

    static void setHookEnabled(const std::string&, bool);

    static bool isLevelInArray(CCArray*, GJGameLevel*);
    
};