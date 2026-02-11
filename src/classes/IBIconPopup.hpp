#include <Geode/ui/Popup.hpp>
#include <span>

class IBIconPopup : public geode::Popup {
protected:
    std::vector<SimplePlayer*> m_simplePlayers;
    std::span<const int> m_ids;
    IconType m_iconType;
    UnlockType m_unlockType;
    int m_page;

    bool init(GJUserScore*, IconType);
    void onPrevPage(cocos2d::CCObject*);
    void onNextPage(cocos2d::CCObject*);
    void onIcon(cocos2d::CCObject*);
    void loadPage(int);
public:
    static IBIconPopup* create(GJUserScore*, IconType);

    void keyDown(cocos2d::enumKeyCodes, double) override;
};
