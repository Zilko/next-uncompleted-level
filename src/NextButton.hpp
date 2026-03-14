#include <Geode/ui/Button.hpp>

#include "Includes.hpp"

class NextButton : public Button {

private:

    CCSprite* m_spr1 = nullptr;
    CCSpriteGrayscale* m_spr2 = nullptr;
    CCSpriteGrayscale* m_spr3 = nullptr;
    CCSprite* m_loadingCircle = nullptr;

    bool myInit(ButtonCallback);

public:

    static NextButton* create(ButtonCallback);

    void setEnabled(bool) override;

    void setScalePro(float);

    void addLoadingCircle();

};