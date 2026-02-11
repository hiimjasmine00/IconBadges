#include <Geode/Enums.hpp>
#include <vector>
#include <map>

namespace IconBadges {
    constexpr std::array badgeNames = { "icon", "ship", "ball", "bird", "dart", "robot", "spider", "swing", "jetpack" };

    extern std::map<int, std::map<IconType, std::vector<int>>> badges;
    extern bool enabled;
}
