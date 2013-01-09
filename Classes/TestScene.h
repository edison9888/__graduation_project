#ifndef TESTSCENE_H
#define TESTSCENE_H

#include <cocos2d.h>

USING_NS_CC;

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

  void buttonA_clicked(CCNode *node);
  void buttonB_clicked(CCNode *node);

  void update (float dt);
};

#endif // TESTSCENE_H
