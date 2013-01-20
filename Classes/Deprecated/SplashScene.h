#ifndef LOADINGSCENE_H
#define LOADINGSCENE_H
#include <cocos2d.h>

USING_NS_CC;

class Splash : public CCLayer
{
public:
  virtual bool init();

  static CCScene* scene();

  void onEnter ();

private:
  void popScene(CCNode* object);

  // implement the "static node()" method manually
  CREATE_FUNC(Splash);
};

#endif // LOADINGSCENE_H
