#include "../IconBadges.hpp"
#include "../classes/IBLeaderboardLayer.hpp"
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

class $modify(IBMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (auto bottomMenu = getChildByID("bottom-menu")) {
            auto leaderboardSprite = CircleButtonSprite::createWithSprite(
                "IB_leaderboardBtn_001.png"_spr, 1.1f, CircleBaseColor::Green, CircleBaseSize::MediumAlt);
            leaderboardSprite->getTopNode()->setPosition({ 27.0f, 27.0f });
            auto leaderboardButton = CCMenuItemSpriteExtra::create(leaderboardSprite, this, menu_selector(IBMenuLayer::onIconLeaderboard));
            leaderboardButton->setID("icon-leaderboard-button"_spr);
            bottomMenu->addChild(leaderboardButton);
            bottomMenu->updateLayout();
        }

        return true;
    }

    void onIconLeaderboard(CCObject*) {
        if (IconBadges::enabled()) {
            CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, IBLeaderboardLayer::scene()));
        }
        else {
            FLAlertLayer::create(
                "GDPS Detected",
                "Please switch to the official <cg>Geometry Dash</c> server to use <cy>Icon Badges</c>.",
                "OK"
            )->show();
        }
    }
};
