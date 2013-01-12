#ifndef TESTSCENE_H
#define TESTSCENE_H

#include <cocos2d.h>
#include <cocos-ext.h>

USING_NS_CC;
USING_NS_CC_EXT;

class TestLayer : public CCLayerColor
{
public:
  TestLayer() {}

  virtual bool init();

  static CCScene *scene()
  {
      CCScene *scene = new CCScene;
      TestLayer *layer = new TestLayer;

      layer->init ();
      ((CCLayerColor *) layer)->autorelease ();
      scene->addChild (layer);

      return scene;
  }

private:
  void buttonA_clicked(CCObject* object, CCControlEvent event);
  void buttonB_clicked(CCObject* object, CCControlEvent event);
  void popScene(CCObject* object);

  void update (float dt);
};

#endif // TESTSCENE_H
