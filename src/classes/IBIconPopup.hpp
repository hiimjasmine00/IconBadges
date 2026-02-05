#include <Geode/ui/Popup.hpp>
#include <span>

class IBIconPopup : public geode::Popup {
protected:
    geode::Ref<cocos2d::CCArray> m_simplePlayers;
    std::span<const int> m_ids;
    IconType m_iconType;
    int m_page;

    bool init(GJUserScore*, IconType);
    void loadPage(int);
public:
    static IBIconPopup* create(GJUserScore*, IconType);

    void keyDown(cocos2d::enumKeyCodes, double) override;
};
