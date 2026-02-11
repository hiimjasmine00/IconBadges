#include "IBBadgePopup.hpp"
#include "IBIconPopup.hpp"
#include "../IconBadges.hpp"
#include <Geode/binding/GJUserScore.hpp>
#include <jasmine/string.hpp>

using namespace geode::prelude;

IBBadgePopup* IBBadgePopup::create(GJUserScore* score) {
    auto ret = new IBBadgePopup();
    if (ret->init(score)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool IBBadgePopup::init(GJUserScore* score) {
    if (!Popup::init(300.0f, 70.0f)) return false;

    setID("IBBadgePopup");
    setTitle(fmt::format("{}'s Badges", JASMINE_STRING(score->m_userName)), "goldFont.fnt", 0.7f, 15.0f);
    m_title->setID("badges-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;

    m_score = score;

    auto badgeMenu = CCMenu::create();
    badgeMenu->setPosition({ 150.0f, 25.0f });
    badgeMenu->setContentSize({ 300.0f, 25.0f });
    badgeMenu->setLayout(RowLayout::create()->setGap(10.0f));
    badgeMenu->setID("badge-menu");

    for (auto& [type, ids] : IconBadges::badges[score->m_accountID]) {
        if (ids.empty()) continue;
        auto badgeName = IconBadges::badgeNames[(int)type];
        auto badgeSprite = CCSprite::createWithSpriteFrameName(fmt::format("gj_{}Btn_on_001.png", badgeName).c_str());
        badgeSprite->setScale(0.7f);
        auto badgeButton = CCMenuItemSpriteExtra::create(badgeSprite, this, menu_selector(IBBadgePopup::onBadge));
        badgeButton->setTag((int)type);
        badgeButton->setID(fmt::format("{}-badge-button", badgeName));
        badgeMenu->addChild(badgeButton);
    }

    badgeMenu->updateLayout();
    m_mainLayer->addChild(badgeMenu);

    return true;
}

void IBBadgePopup::onBadge(CCObject* sender) {
    IBIconPopup::create(m_score, (IconType)sender->getTag())->show();
}
