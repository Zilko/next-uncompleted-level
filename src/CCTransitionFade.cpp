#include "CCTransitionFade.hpp"
#include "Manager.hpp"

CCTransitionFade* ProCCTransitionFade::create(float time, CCScene* scene) {
    if (Manager::get().goingToLevel) {
        time = 0.f;
    }

    return CCTransitionFade::create(time, scene);
}