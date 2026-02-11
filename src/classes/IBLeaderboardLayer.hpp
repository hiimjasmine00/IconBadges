#include <cocos2d.h>
#include <Geode/Enums.hpp>
#include <Geode/GeneratedPredeclare.hpp>

class IBLeaderboardLayer : public cocos2d::CCLayer {
protected:
    GJListLayer* m_listLayer;

    bool init() override;
    void onRefresh(cocos2d::CCObject*);
    void onType(cocos2d::CCObject*);
    void updateList();
    void onClose(cocos2d::CCObject*);
public:
    static IBLeaderboardLayer* create();
    static cocos2d::CCScene* scene();

    void keyBackClicked() override;
};
