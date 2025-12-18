#include "Includes.hpp"

#include <Geode/modify/LevelInfoLayer.hpp>

class $modify(ProLevelInfoLayer, LevelInfoLayer) {
    
    struct Fields {
      
        LevelBrowserLayer* m_browserLayer = nullptr;
        
    };

    void onEnterTransitionDidFinish();
  
    bool init(GJGameLevel*, bool);
    
    void onPlay(CCObject*);
    
};