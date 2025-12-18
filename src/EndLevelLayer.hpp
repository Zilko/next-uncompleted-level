#include "Includes.hpp"

#include <Geode/modify/EndLevelLayer.hpp>

class $modify(ProEndLevelLayer, EndLevelLayer) {

    struct Fields {
        CCMenuItemSpriteExtra* m_button = nullptr;
        GJGameLevel* m_nextLevel = nullptr;
        CCSprite* m_spr1 = nullptr;
        CCSpriteGrayscale* m_spr2 = nullptr;
        CCSpriteGrayscale* m_spr3 = nullptr;
        CCSprite* m_loadingCircle = nullptr;
    };

    void onNextLevel(CCObject*);
  
    void setButtonEnabled(bool);
    
    void customSetup();
    
};