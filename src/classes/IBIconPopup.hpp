#include <Geode/ui/Popup.hpp>
#include <span>

class IBIconPopup : public geode::Popup<GJUserScore*, IconType, const std::vector<int>&> {
protected:
    geode::Ref<cocos2d::CCArray> m_simplePlayers;
    std::span<const int> m_ids;
    IconType m_iconType;
    int m_page;

    bool setup(GJUserScore*, IconType, const std::vector<int>&) override;
    void loadPage(int);
public:
    static IBIconPopup* create(GJUserScore*, IconType, const std::vector<int>&);

    void keyDown(cocos2d::enumKeyCodes) override;
};
