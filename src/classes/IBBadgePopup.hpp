#include <Geode/ui/Popup.hpp>

class IBBadgePopup : public geode::Popup {
protected:
    bool init(GJUserScore*);
public:
    static IBBadgePopup* create(GJUserScore*);
};
