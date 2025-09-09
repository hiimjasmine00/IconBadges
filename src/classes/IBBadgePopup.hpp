#include <Geode/ui/Popup.hpp>

class IBBadgePopup : public geode::Popup<GJUserScore*, const std::map<IconType, std::vector<int>>&> {
protected:
    bool setup(GJUserScore*, const std::map<IconType, std::vector<int>>&) override;
public:
    static IBBadgePopup* create(GJUserScore*, const std::map<IconType, std::vector<int>>&);
};
