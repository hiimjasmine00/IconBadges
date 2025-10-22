#include "IBLeaderboardLayer.hpp"
#include "IBBadgePopup.hpp"
#include "../IconBadges.hpp"
#include <Geode/binding/AppDelegate.hpp>
#include <Geode/binding/CustomListView.hpp>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/GameStatsManager.hpp>
#include <Geode/binding/GJListLayer.hpp>
#include <Geode/binding/GJUserScore.hpp>
#include <Geode/binding/MenuLayer.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

std::map<IconType, bool> typesEnabled = {
    { IconType::Cube, false },
    { IconType::Ship, false },
    { IconType::Ball, false },
    { IconType::Ufo, false },
    { IconType::Wave, false },
    { IconType::Robot, false },
    { IconType::Spider, false },
    { IconType::Swing, false },
    { IconType::Jetpack, false }
};

IBLeaderboardLayer* IBLeaderboardLayer::create() {
    auto ret = new IBLeaderboardLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

CCScene* IBLeaderboardLayer::scene() {
    auto ret = CCScene::create();
    AppDelegate::get()->m_runningScene = ret;
    ret->addChild(IBLeaderboardLayer::create());
    return ret;
}

bool IBLeaderboardLayer::init() {
    if (!CCLayer::init()) return false;

    setID("IBLeaderboardLayer");

    auto winSize = CCDirector::get()->getWinSize();
    auto center = winSize / 2.0f;

    auto bg = CCSprite::create("GJ_gradientBG.png");
    bg->setScaleX((winSize.width + 10.0f) / bg->getTextureRect().size.width);
    bg->setScaleY((winSize.height + 10.0f) / bg->getTextureRect().size.height);
    bg->setPosition(center);
    bg->setColor({ 0, 102, 255 });
    bg->setID("background");
    addChild(bg);

    auto leftCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    leftCorner->setPosition({ -1.0f, -1.0f });
    leftCorner->setAnchorPoint({ 0.0f, 0.0f });
    leftCorner->setID("left-corner");
    addChild(leftCorner);

    auto rightCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    rightCorner->setPosition({ winSize.width + 1.0f, -1.0f });
    rightCorner->setAnchorPoint({ 1.0f, 0.0f });
    rightCorner->setFlipX(true);
    rightCorner->setID("right-corner");
    addChild(rightCorner);

    auto buttonMenu = CCMenu::create();
    buttonMenu->ignoreAnchorPointForPosition(false);
    buttonMenu->setID("back-menu");
    addChild(buttonMenu);

    auto backButton = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_arrow_01_001.png", 1.0f, [this](auto) {
        close();
    });
    backButton->setPosition({ 24.0f, winSize.height - 23.0f });
    backButton->setID("back-button");
    buttonMenu->addChild(backButton);

    auto refreshBtnSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    auto width = refreshBtnSpr->getTextureRect().size.width / 2.0f + 4.0f;
    auto refreshButton = CCMenuItemExt::createSpriteExtra(refreshBtnSpr, [this](auto) {
        updateList();
    });
    refreshButton->setPosition({ winSize.width - width, width });
    refreshButton->setID("refresh-button");
    buttonMenu->addChild(refreshButton);

    m_listLayer = GJListLayer::create(nullptr, "Icon Leaderboard", { 191, 114, 62, 255 }, 356.0f, 220.0f, 0);
    m_listLayer->ignoreAnchorPointForPosition(false);
    m_listLayer->setPosition(center - CCPoint { 0.0f, 5.0f });
    m_listLayer->setID("list-layer");
    addChild(m_listLayer);

    constexpr std::array badgeNames = { "icon", "ship", "ball", "bird", "dart", "robot", "spider", "swing", "jetpack" };

    auto badgeMenu = CCMenu::create();
    badgeMenu->setPosition(center + CCPoint { 230.0f, 15.0f });
    badgeMenu->setContentSize({ 60.0f, 170.0f });
    badgeMenu->setLayout(ColumnLayout::create()
        ->setGap(8.0f)
        ->setAxisReverse(true)
        ->setCrossAxisReverse(true)
        ->setGrowCrossAxis(true)
        ->setAxisAlignment(AxisAlignment::End));
    badgeMenu->setID("badge-menu");

    for (int i = 0; i < 9; i++) {
        auto badgeName = badgeNames[i];
        auto badgeButton = CCMenuItemExt::createSpriteExtraWithFrameName(
            fmt::format("gj_{}Btn_{}_001.png", badgeName, typesEnabled[(IconType)i] ? "on" : "off"), 0.8f,
            [this, badgeName](CCMenuItemSpriteExtra* sender) {
                auto& typeEnabled = typesEnabled[(IconType)sender->getTag()];
                typeEnabled = !typeEnabled;
                static_cast<CCSprite*>(sender->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(
                    fmt::format("gj_{}Btn_{}_001.png", badgeName, typeEnabled ? "on" : "off").c_str()));
                updateList();
            }
        );
        badgeButton->setTag(i);
        badgeButton->setID(fmt::format("{}-badge-button", badgeName));
        badgeMenu->addChild(badgeButton);
    }

    badgeMenu->updateLayout();
    addChild(badgeMenu);

    setKeypadEnabled(true);
    updateList();

    return true;
}

void IBLeaderboardLayer::updateList() {
    if (auto listView = m_listLayer->m_listView) {
        listView->removeFromParent();
        listView->release();
    }

    std::vector<std::pair<int, int>> rankMap;
    auto enabledOverride = std::ranges::all_of(typesEnabled, [](const std::pair<IconType, bool>& pair) {
        return !pair.second;
    });
    for (auto& [type, enabled] : typesEnabled) {
        if (!enabled && !enabledOverride) continue;
        for (auto& [accountID, icons] : IconBadges::badges) {
            auto it = std::ranges::find_if(rankMap, [accountID](const std::pair<int, int>& pair) {
                return pair.first == accountID;
            });
            if (it == rankMap.end()) {
                rankMap.emplace_back(accountID, 0);
                it = rankMap.end() - 1;
            }
            if (icons.contains(type)) {
                it->second += icons[type].size();
            }
        }
    }
    std::ranges::sort(rankMap, [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;
    });

    auto scores = CCArray::create();
    auto glm = GameLevelManager::get();
    auto& usernameForAccountID = GameStatsManager::get()->m_usernameForAccountID;
    for (int i = 0; i < rankMap.size(); i++) {
        auto& [accountID, count] = rankMap[i];
        if (count <= 0) continue;
        auto score = glm->userInfoForAccountID(accountID);
        if (!score) {
            score = GJUserScore::create();
            score->m_accountID = accountID;
            if (accountID == 71) score->m_userName = "RobTop";
            else {
                auto it = usernameForAccountID.find(accountID);
                score->m_userName = it != usernameForAccountID.end() ? it->second : "[No Name]";
            }
        }
        score->setUserObject("icon-count"_spr, CCString::create(fmt::to_string(count)));
        score->setUserObject("icon-id"_spr, CCInteger::create(score->m_iconID));
        score->setUserObject("player-rank"_spr, CCInteger::create(score->m_playerRank));
        score->m_iconID = std::max(score->m_playerCube, 1);
        score->m_playerRank = i + 1;
        scores->addObject(score);
    }

    auto listView = CustomListView::create(scores, BoomListType::Score, 220.0f, 356.0f);
    listView->retain();
    m_listLayer->addChild(listView, 6, 9);
    m_listLayer->m_listView = listView;

    for (auto score : CCArrayExt<GJUserScore*>(scores)) {
        score->m_iconID = static_cast<CCInteger*>(score->getUserObject("icon-id"_spr))->getValue();
        score->m_playerRank = static_cast<CCInteger*>(score->getUserObject("player-rank"_spr))->getValue();
        score->setUserObject("icon-count"_spr, nullptr);
        score->setUserObject("icon-id"_spr, nullptr);
        score->setUserObject("player-rank"_spr, nullptr);
    }
}

void IBLeaderboardLayer::close() {
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, MenuLayer::scene(false)));
}

void IBLeaderboardLayer::keyBackClicked() {
    close();
}
