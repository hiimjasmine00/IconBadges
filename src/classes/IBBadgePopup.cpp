#include "IBBadgePopup.hpp"
#include "IBIconPopup.hpp"
#include "../IconBadges.hpp"
#include <Geode/binding/GJUserScore.hpp>

using namespace geode::prelude;

IBBadgePopup* IBBadgePopup::create(GJUserScore* score) {
    auto ret = new IBBadgePopup();
    if (ret->initAnchored(300.0f, 70.0f, score)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool IBBadgePopup::setup(GJUserScore* score) {
    setID("IBBadgePopup");
    setTitle(fmt::format("{}'s Badges", GEODE_ANDROID(std::string)(score->m_userName)), "goldFont.fnt", 0.7f, 15.0f);
    m_title->setID("badges-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;

    constexpr std::array badgeNames = { "icon", "ship", "ball", "bird", "dart", "robot", "spider", "swing", "jetpack" };

    auto badgeMenu = CCMenu::create();
    badgeMenu->setPosition({ 150.0f, 25.0f });
    badgeMenu->setContentSize({ 300.0f, 25.0f });
    badgeMenu->setLayout(RowLayout::create()->setGap(10.0f));
    badgeMenu->setID("badge-menu");

    for (auto& [type, ids] : IconBadges::badges[score->m_accountID]) {
        if (ids.empty()) continue;
        auto badgeName = badgeNames[(int)type];
        auto badgeButton = CCMenuItemExt::createSpriteExtraWithFrameName(fmt::format("gj_{}Btn_on_001.png", badgeName), 0.7f, [
            scoreRef = WeakRef(score), type
        ](auto) {
            if (auto score = scoreRef.lock()) IBIconPopup::create(score, type)->show();
        });
        badgeButton->setID(fmt::format("{}-badge-button", badgeName));
        badgeMenu->addChild(badgeButton);
    }

    badgeMenu->updateLayout();
    m_mainLayer->addChild(badgeMenu);

    return true;
}
