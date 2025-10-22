#include <Geode/ui/Popup.hpp>

class IBBadgePopup : public geode::Popup<GJUserScore*> {
protected:
    bool setup(GJUserScore*) override;
public:
    static IBBadgePopup* create(GJUserScore*);
};
