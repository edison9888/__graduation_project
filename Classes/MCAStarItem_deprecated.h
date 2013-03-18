#ifndef __MCAStarItem_H
#define __MCAStarItem_H

#include <cocos2d.h>
USING_NS_CC;

class MCAStarAlgorithm_deprecated;

class MCAStarItem_deprecated : public CCObject {
    friend class MCAStarAlgorithm_deprecated;
public:
    void setPosition(int col, int row);
    
    inline CCPoint getPosition() {
        return ccp(col_, row_);
    }

	CC_SYNTHESIZE(int, col_, Col);
	CC_SYNTHESIZE(int, row_, Row);
	CC_SYNTHESIZE(int, g_, G);
	CC_SYNTHESIZE(int, h_, H);
	CC_SYNTHESIZE(int, f_, F);
    CC_SYNTHESIZE(int, parent_, PID);
};

#endif
