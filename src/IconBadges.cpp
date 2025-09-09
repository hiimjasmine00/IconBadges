#include "IconBadges.hpp"
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/GameStatsManager.hpp>
#ifdef GEODE_IS_ANDROID
#include <Geode/binding/GJMoreGamesLayer.hpp>
#endif
#include <Geode/loader/Loader.hpp>
#include <km7dev.server_api/include/ServerAPIEvents.hpp>

using namespace geode::prelude;

bool IconBadges::enabled() {
    static bool enabled = [] {
        std::string url;
        auto foundURL = false;
        if (Loader::get()->isModLoaded("km7dev.server_api")) {
            url = ServerAPIEvents::getCurrentServer().url;
            if (!url.empty() && url != "NONE_REGISTERED") {
                for (; url.ends_with("/"); url = url.substr(0, url.size() - 1));
                foundURL = true;
            }
        }

        if (!foundURL) {
            url = std::string(reinterpret_cast<const char*>(base::get() +
                GEODE_WINDOWS(0x53ea48)
                GEODE_ARM_MAC(0x7749fb)
                GEODE_INTEL_MAC(0x8516bf)
                GEODE_ANDROID64((((GJMoreGamesLayer* volatile)nullptr)->getMoreGamesList()->count() > 0 ? 0xea2988 : 0xea27f8))
                GEODE_ANDROID32((((GJMoreGamesLayer* volatile)nullptr)->getMoreGamesList()->count() > 0 ? 0x952e9e : 0x952cce))
                GEODE_IOS(0x6af51a)
            ), 0, 34);
        }

        return url.rfind("://www.boomlings.com/database") != std::string::npos;
    }();
    return enabled;
}

$on_mod(Loaded) {
    if (!IconBadges::enabled()) return;

    auto gm = GameManager::get();
    auto gsm = GameStatsManager::get();
    auto& accountIDForIcon = gsm->m_accountIDForIcon;
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
