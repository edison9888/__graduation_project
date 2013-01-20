//
//  TestScene.cpp
//  Military Confrontation
//
//  Created by 江宇英 on 13-1-19.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "ccConfig.h"
#include "MCTestScene.h"

bool
MCTestLayer::init()
{
    if (CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
    {
        setTouchEnabled(true);
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("mc_crow_cocos2dx.plist");
        CCSpriteBatchNode* spriteSheet = CCSpriteBatchNode::create("mc_crow_cocos2dx.png");
        
        this->addChild(spriteSheet);
        
        CCArray *flyToLeftAnimFrames = CCArray::create();
        char str[64];
        for(int i = 0; i < 4; i++)
        {
            sprintf(str, "mc_crow_left_fly_%d.png", i);
            CCSpriteFrame *spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
            flyToLeftAnimFrames->addObject(spriteFrame);
        }
        CCAnimation * animation = CCAnimation::createWithSpriteFrames(flyToLeftAnimFrames, 0.1f);
        
        CCSpriteFrame *spriteFrame = (CCSpriteFrame *) flyToLeftAnimFrames->objectAtIndex(0);
        CCSprite* crow = CCSprite::createWithSpriteFrame(spriteFrame);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        crow->setPosition(ccp(size.width / 2, size.height / 2));
        CCActionInterval* walkAction = CCRepeatForever::create(CCAnimate::create(animation));
        spriteSheet->addChild(crow);
        crow->runAction(walkAction);
        
        _crow = crow;
        
//        crow = CCSprite::createWithSpriteFrame((CCSpriteFrame *) flyToLeftAnimFrames->objectAtIndex(0));
//        crow->setPosition(ccp(size.width / 2 + 32, size.height / 2 + 32));
//        walkAction = CCRepeatForever::create(CCAnimate::create(animation));
//        spriteSheet->addChild(crow);
//        crow->runAction(walkAction);
        
        CCTMXTiledMap *map = CCTMXTiledMap::create("tileMap.tmx");
        this->addChild(map, 0, 1);
        map->setPosition(ccp(0, 0));
        spriteSheet->setZOrder(1);
//        CCTexture2D* texture2D = CCTextureCache::sharedTextureCache()->textureForKey("tmw_desert_spacing.png");
//        texture2D->setAliasTexParameters();
        
        return true;
    }
    
    return false;
}

void MCTestLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool MCTestLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    // _prevousLocation = pTouch->getLocation();
    
	return true;
}

void MCTestLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint touchLocation = pTouch->getLocationInView();
	CCPoint prevLocation = pTouch->getPreviousLocationInView();
    
	touchLocation  = CCDirector::sharedDirector()->convertToGL(touchLocation);
	prevLocation  = CCDirector::sharedDirector()->convertToGL(prevLocation);
    
	CCPoint diff = ccpSub(touchLocation, prevLocation);
    
	CCTMXTiledMap* map = (CCTMXTiledMap *) getChildByTag(1);
	CCPoint currentPos = map->getPosition();
    CCPoint newPos = ccpAdd(currentPos, diff);
    CCSize mapSize = map->getMapSize();
    CCSize tileSize = map->getTileSize();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float contentScaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
    float w = mapSize.width * tileSize.width;
    float h = mapSize.height * tileSize.height;
    float ww = winSize.width * contentScaleFactor;
    float wh = winSize.height * contentScaleFactor;
    if (newPos.x > 0) {
        newPos.x = 0;
    } else if (newPos.x - ww < -w) {
        newPos.x = ww - w;
    }
    if (newPos.y > 0) {
        newPos.y = 0;
    } else if (newPos.y - wh < -h) {
        newPos.y = wh - h;
    }
    map->setPosition(newPos);
//   _crow->setPosition(ccpAdd(_crow->getPosition(), diff));
}

void MCTestLayer::ccTouchCancelled(CCTouch* touch, CCEvent* event)
{
}

void MCTestLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchLocation = pTouch->getLocation();
    
    // if (! (_prevousLocation.x == touchLocation.x && _prevousLocation.y == touchLocation.y)) {
    //     return;
    // }
//    return;
    
    CCPoint nowLocation = _crow->getPosition();
    float crowVelocity = 480.0/3.0;
    CCPoint moveDifference = ccpSub(touchLocation, nowLocation);
    float distanceToMove = ccpLength(moveDifference);
    float moveDuration = distanceToMove / crowVelocity;
    
    if (moveDifference.x < 0) {
        _crow->setFlipX(false);
    } else {
        _crow->setFlipX(true);
    }
    
    _crow->stopAction(_moveAction);
    
    _moveAction = CCMoveTo::create(moveDuration, touchLocation);
    
    _crow->runAction(_moveAction);

}

void
MCTestLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
}
