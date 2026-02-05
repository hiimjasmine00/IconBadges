#include "IconBadges.hpp"
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/GameStatsManager.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <jasmine/gdps.hpp>

using namespace geode::prelude;

std::map<int, std::map<IconType, std::vector<int>>> IconBadges::badges;

bool IconBadges::enabled = jasmine::gdps::isActive();

$on_mod(Loaded) {
    if (!IconBadges::enabled) return;

    auto gm = GameManager::get();
    auto& accountIDForIcon = GameStatsManager::get()->m_accountIDForIcon;
    for (int i = 0; i < 9; i++) {
        auto iconType = (IconType)i;
        auto count = gm->countForType(iconType);
        auto unlockType = gm->iconTypeToUnlockType(iconType);
        for (int j = 1; j <= count; j++) {
            auto it = accountIDForIcon.find({ j, unlockType });
            IconBadges::badges[it != accountIDForIcon.end() ? it->second : 71][iconType].push_back(j);
        }
    }
}
