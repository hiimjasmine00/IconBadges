#include "IBIconPopup.hpp"
#include "../IconBadges.hpp"
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/GJItemIcon.hpp>
#include <Geode/binding/GJSpiderSprite.hpp>
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/binding/ItemInfoPopup.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <jasmine/string.hpp>

using namespace geode::prelude;

IBIconPopup* IBIconPopup::create(GJUserScore* score, IconType type) {
    auto ret = new IBIconPopup();
    if (ret->init(score, type)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool IBIconPopup::init(GJUserScore* score, IconType type) {
    if (!Popup::init(300.0f, 250.0f)) return false;

    constexpr std::array iconNames = { "Cube", "Ship", "Ball", "UFO", "Wave", "Robot", "Spider", "Swing", "Jetpack" };

    auto& ids = IconBadges::badges[score->m_accountID][type];
    auto size = ids.size();

    setID("IBIconPopup");
    setTitle(fmt::format("{}'s {} {}{}", JASMINE_STRING(score->m_userName), size, iconNames[(int)type], size == 1 ? "" : "s"));
    m_title->setID("icons-title");
    m_mainLayer->setID("main-layer");
    m_buttonMenu->setID("button-menu");
    m_bgSprite->setID("background");
    m_closeBtn->setID("close-button");
    m_noElasticity = true;

    m_simplePlayers = CCArray::create();
    m_ids = ids;
    m_iconType = type;

    auto iconBackground = CCScale9Sprite::create("square02_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    iconBackground->setPosition({ 150.0f, 115.0f });
    iconBackground->setContentSize({ 270.0f, 200.0f });
    iconBackground->setOpacity(105);
    iconBackground->setID("icon-background");
    m_mainLayer->addChild(iconBackground);

    auto playerSquare = CCSpriteFrameCache::get()->spriteFrameByName("playerSquare_001.png");
    auto originalSize = playerSquare ? playerSquare->getOriginalSize() : CCSize { 0.0f, 0.0f };
    auto center = originalSize / 2.0f;
    auto gameManager = GameManager::get();
    auto unlockType = gameManager->iconTypeToUnlockType(type);
    auto color1 = gameManager->colorForIdx(score->m_color1);
    auto color2 = gameManager->colorForIdx(score->m_color2);
    auto color3 = gameManager->colorForIdx(score->m_color3);
    auto glow = score->m_glowEnabled;
    auto scale = GJItemIcon::scaleForType(unlockType) * 1.2f;
    auto count = std::min<int>(size, 30);
    for (int i = 0; i < count; i++) {
        auto simplePlayer = SimplePlayer::create(1);
        simplePlayer->updatePlayerFrame(1, type);
        simplePlayer->setColor(color1);
        simplePlayer->setSecondColor(color2);
        simplePlayer->enableCustomGlowColor(color3);
        simplePlayer->m_hasGlowOutline = glow;
        simplePlayer->updateColors();
        simplePlayer->setScale(scale);

        auto iconButton = CCMenuItemExt::createSpriteExtra(simplePlayer, [unlockType](CCMenuItemSpriteExtra* sender) {
            ItemInfoPopup::create(sender->getTag(), unlockType)->show();
        });
        iconButton->setPosition({ 50.0f + (i % 6) * 40.0f, 195.0f - (int)(i / 6) * 40.0f });
        simplePlayer->setPosition(center);
        iconButton->setContentSize(originalSize);
        iconButton->setID(fmt::format("icon-{}", i));
        m_simplePlayers->addObject(simplePlayer);
        m_buttonMenu->addChild(iconButton);
    }

    if (size > count) {
        auto prevButton = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_arrow_01_001.png", 1.0f, [this](auto) {
            loadPage(m_page - 1);
        });
        prevButton->setPosition({ -25.0f, 125.0f });
        prevButton->setID("prev-button");
        m_buttonMenu->addChild(prevButton);

        auto nextButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        nextButtonSprite->setFlipX(true);
        auto nextButton = CCMenuItemExt::createSpriteExtra(nextButtonSprite, [this](auto) {
            loadPage(m_page + 1);
        });
        nextButton->setPosition({ 325.0f, 125.0f });
        nextButton->setID("next-button");
        m_buttonMenu->addChild(nextButton);
    }

    loadPage(0);
    setKeyboardEnabled(true);

    return true;
}

void IBIconPopup::loadPage(int page) {
    int size = m_ids.size();
    auto pages = (size + 29) / 30;
    if (pages == 0) return;

    m_page = std::clamp(pages > 0 ? (pages + (page % pages)) % pages : 0, 0, pages - 1);
    auto index = m_page * 30;
    if (size <= index) return;

    auto count = m_simplePlayers->count();
    auto pageSpan = m_ids.subspan(index, std::min<int>(30, size - index));
    for (int i = 0; i < count; i++) {
        auto simplePlayer = static_cast<SimplePlayer*>(m_simplePlayers->objectAtIndex(i));
        auto playerButton = simplePlayer->getParent();
        auto visible = i < pageSpan.size();
        playerButton->setVisible(visible);
        if (visible) {
            auto id = pageSpan[i];
            playerButton->setTag(id);
            simplePlayer->updatePlayerFrame(id, m_iconType);
        }
    }
}

void IBIconPopup::keyDown(enumKeyCodes key, double timestamp) {
    switch (key) {
        case KEY_Left: case CONTROLLER_Left: return loadPage(m_page - 1);
        case KEY_Right: case CONTROLLER_Right: return loadPage(m_page + 1);
        default: return Popup::keyDown(key, timestamp);
    }
}
