#include "../IconBadges.hpp"
#include "../classes/IBBadgePopup.hpp"
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <ranges>

using namespace geode::prelude;

class $modify(IBProfilePage, ProfilePage) {
    struct Fields {
        bool m_hasBadge;
    };

    static void onModify(ModifyBase<ModifyDerive<IBProfilePage, ProfilePage>>& self) {
        for (auto& hook : std::views::values(self.m_hooks)) {
            hook->setAutoEnable(IconBadges::enabled);
        }
    }

    void loadPageFromUserInfo(GJUserScore* score) {
        ProfilePage::loadPageFromUserInfo(score);

        auto f = m_fields.self();
        if (f->m_hasBadge) return;

        auto usernameMenu = m_mainLayer->getChildByID("username-menu");
        if (!usernameMenu) return;

        if (IconBadges::badges.contains(score->m_accountID)) {
            auto badgeSprite = CCSprite::createWithSpriteFrameName("gj_iconBtn_off_001.png");
            badgeSprite->setScale(0.65f);
            auto iconBadge = CCMenuItemSpriteExtra::create(badgeSprite, this, menu_selector(IBProfilePage::onIconBadge));
            iconBadge->setID("icon-badge"_spr);
            usernameMenu->addChild(iconBadge);
            usernameMenu->updateLayout();
            f->m_hasBadge = true;
        }
    }

    void onIconBadge(CCObject*) {
        IBBadgePopup::create(m_score)->show();
    }
};
