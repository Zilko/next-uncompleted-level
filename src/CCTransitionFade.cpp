#include "CCTransitionFade.hpp"
#include "Manager.hpp"

CCTransitionFade* ProCCTransitionFade::create(float time, CCScene* scene) {
    auto didTroll = false;

    if (++Manager::get().transitionCount >= 4) {
        didTroll = true;
        Manager::get().goingToLevel = nullptr;
        setHookEnabled("cocos2d::CCTransitionFade::create", false);
    }

    if (Manager::get().goingToLevel) {
        time = 0.f;

        if (didTroll) {
            Notification::create("Next Uncompleted Level: An error occurred.");
        }
    }

    return CCTransitionFade::create(time, scene);
}