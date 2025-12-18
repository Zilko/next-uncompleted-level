#pragma once

#include "Includes.hpp"

#include <Geode/modify/LevelBrowserLayer.hpp>

class $modify(ProLevelBrowserLayer, LevelBrowserLayer) {
  
    struct Fields {
      
        LevelBrowserLayer* m_self = nullptr;
        
        GJGameLevel* m_currentLevel = nullptr;

        EndLevelLayer* m_endLayer = nullptr;
        
        int m_skippedPages = 0;
        int m_allowedAttempts = 0;
        
        ~Fields();
        
    };
    
    void onEnterTransitionDidFinish();
    void loadLevelsFinished(CCArray*, const char*, int);
    void loadLevelsFailed(const char*, int);
    
};