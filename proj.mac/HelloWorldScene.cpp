#include "HelloWorldScene.h"

using namespace cocos2d;
const int tagTileMap = 1;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
       // HelloWorld *layer = HelloWorld::node();
		MapLayer* layer = new MapLayer();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".
        CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 64);
        CC_BREAK_IF(! pLabel);

        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width / 2, size.height - 20));

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("HelloWorld.png");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}



/************************************************************************/
/*  MapLayer                                                                    */
/************************************************************************/
MapLayer::MapLayer()
{
	setTouchEnabled(true);
	CCTMXTiledMap* map  = CCTMXTiledMap::create("tileMap.tmx");
	addChild(map, 1, tagTileMap);
	map->setPosition(ccp(0, 0));

	CCTMXObjectGroup* objGroup = map->objectGroupNamed("objects");
	CCDictionary *spawnPoint = objGroup->objectNamed("spawnpoint");
	float x = spawnPoint->valueForKey("x")->floatValue();
	float y = spawnPoint->valueForKey("y")->floatValue();

	CCTexture2D* playerTexture = CCTextureCache::sharedTextureCache()->addImage("Player.png");

	m_player = CCSprite::createWithTexture(playerTexture);
	m_player->retain();
	m_player->setPosition(ccp(x, y));


	map->addChild(m_player);
}

MapLayer::~MapLayer()
{
	m_player->release();
}

void MapLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool MapLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}

void MapLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint touchLocation = pTouch->getLocationInView();
	CCPoint prevLocation = pTouch->getPreviousLocationInView();

	touchLocation  = CCDirector::sharedDirector()->convertToGL(touchLocation);
	prevLocation  = CCDirector::sharedDirector()->convertToGL(prevLocation);

	CCPoint diff = ccpSub(touchLocation, prevLocation);

	CCNode* node = getChildByTag(tagTileMap);
	CCPoint currentPos = node->getPosition();
	node->setPosition(ccpAdd(currentPos, diff));


}

void MapLayer::ccTouchCancelled(CCTouch* touch, CCEvent* event)
{
}

void MapLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint location = pTouch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	
	CCActionInterval* action = CCMoveTo::create(3, location);
	m_player->runAction(action);
}