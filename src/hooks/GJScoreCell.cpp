#include "../IconBadges.hpp"
#include "../classes/IBBadgePopup.hpp"
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/modify/GJScoreCell.hpp>
#include <jasmine/hook.hpp>

using namespace geode::prelude;

class $modify(IBScoreCell, GJScoreCell) {
    static void onModify(ModifyBase<ModifyDerive<IBScoreCell, GJScoreCell>>& self) {
        jasmine::hook::get(self.m_hooks, "GJScoreCell::loadFromScore", IconBadges::enabled);
    }

    void loadFromScore(GJUserScore* score) {
        GJScoreCell::loadFromScore(score);

        auto iconCount = static_cast<CCString*>(score->getUserObject("icon-count"_spr));
        if (!iconCount) return;

        CCNode* playerIcon = nullptr;
        CCNode* mainMenu = nullptr;
        CCNode* starsLabel = nullptr;
        CCNode* starsIcon = nullptr;
        CCNode* moonsLabel = nullptr;
        CCNode* moonsIcon = nullptr;
        for (auto child : CCArrayExt<CCNode*>(m_mainLayer->getChildren())) {
            auto& id = child->getID();
            if (id == "player-icon") playerIcon = child;
            else if (id == "main-menu") mainMenu = child;
            else if (id == "stars-label") starsLabel = child;
            else if (id == "stars-icon") starsIcon = child;
            else if (id == "moons-label") moonsLabel = child;
            else if (id == "moons-icon") moonsIcon = child;
        }
        if (!playerIcon || !mainMenu || !starsLabel || !starsIcon || !moonsLabel || !moonsIcon) return;

        auto playerName = static_cast<CCMenuItemSprite*>(mainMenu->getChildByID("player-name"));
        if (!playerName) return;

        starsLabel->setScale(starsLabel->getScale() * 0.9f);
        starsLabel->setPosition(playerIcon->getPosition() + CCPoint { playerName->getNormalImage()->getScaledContentWidth() + 35.0f, 5.0f });
        starsIcon->setScale(starsIcon->getScale() * 0.9f);
        starsIcon->setPosition(starsLabel->getPosition() + CCPoint { starsLabel->getScaledContentWidth() + 8.0f, 0.0f });
        moonsLabel->setScale(moonsLabel->getScale() * 0.9f);
        moonsLabel->setPosition(starsIcon->getPosition() + CCPoint { starsIcon->getScaledContentWidth() / 2.0f + 7.0f, 0.0f });
        moonsIcon->setScale(moonsIcon->getScale() * 0.9f);
        moonsIcon->setPosition(moonsLabel->getPosition() + CCPoint { moonsLabel->getScaledContentWidth() + 8.0f, 0.0f });

        auto iconsLabel = CCLabelBMFont::create(iconCount->getCString(), "bigFont.fnt");
        iconsLabel->setPosition(moonsIcon->getPosition() + CCPoint { moonsIcon->getScaledContentWidth() / 2.0f + 7.0f, 0.0f });
        iconsLabel->limitLabelWidth(54.0f, 0.45f - (score->m_creatorPoints > 0) * 0.05f, 0.0f);
        iconsLabel->setAnchorPoint({ 0.0f, 0.5f });
        iconsLabel->setID("icons-label"_spr);
        m_mainLayer->addChild(iconsLabel, 2);

        auto iconsIcon = CCSprite::createWithSpriteFrameName("gj_iconBtn_off_001.png");
        iconsIcon->setScale(0.45f);
        auto iconsButton = CCMenuItemSpriteExtra::create(iconsIcon, this, menu_selector(IBScoreCell::onIcon));
        iconsButton->setPosition(mainMenu->convertToNodeSpace(m_mainLayer->convertToWorldSpace(
            iconsLabel->getPosition() + CCPoint { iconsLabel->getScaledContentWidth() + 8.0f, 0.0f })));
        iconsButton->setID("icons-button"_spr);
        mainMenu->addChild(iconsButton, 2);
    }

    void onIcon(CCObject* sender) {
        if (IconBadges::badges.contains(m_score->m_accountID)) {
            IBBadgePopup::create(m_score)->show();
        }
    }
};
