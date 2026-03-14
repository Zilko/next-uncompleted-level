#include "Includes.hpp"

#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/ui/Button.hpp>

class NextButton;

class $modify(ProEndLevelLayer, EndLevelLayer) {

    struct Fields {
        NextButton* m_button = nullptr;
        GJGameLevel* m_nextLevel = nullptr;
    };

    void onNextLevel();
  
    void setButtonEnabled(bool);
    
    void customSetup();
    
};