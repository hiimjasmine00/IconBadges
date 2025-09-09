#include <cocos2d.h>
#include <Geode/Enums.hpp>
#include <Geode/GeneratedPredeclare.hpp>

class IBLeaderboardLayer : public cocos2d::CCLayer {
protected:
    inline static std::map<IconType, bool> typesEnabled = {
        { IconType::Cube, false },
        { IconType::Ship, false },
        { IconType::Ball, false },
        { IconType::Ufo, false },
        { IconType::Wave, false },
        { IconType::Robot, false },
        { IconType::Spider, false },
        { IconType::Swing, false },
        { IconType::Jetpack, false }
    };
    GJListLayer* m_listLayer;

    bool init() override;
    void updateList();
    void close();
public:
    static IBLeaderboardLayer* create();
    static cocos2d::CCScene* scene();

    void keyBackClicked() override;
};
