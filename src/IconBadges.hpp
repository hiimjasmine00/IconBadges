#include <Geode/Enums.hpp>
#include <vector>
#include <map>

class IconBadges {
public:
    inline static std::map<int, std::map<IconType, std::vector<int>>> badges;

    static bool enabled();
};
