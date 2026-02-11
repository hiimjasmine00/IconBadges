#include <Geode/ui/Popup.hpp>

class IBBadgePopup : public geode::Popup {
protected:
    GJUserScore* m_score;

    bool init(GJUserScore*);
    void onBadge(cocos2d::CCObject*);
public:
    static IBBadgePopup* create(GJUserScore*);
};
