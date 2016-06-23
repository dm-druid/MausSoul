#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

class  AppDelegate : private cocos2d::Application {
public:
	AppDelegate(void);
	virtual ~AppDelegate(void);

	virtual void initGLContextAttrs(void);

	virtual bool applicationDidFinishLaunching(void);

	virtual void applicationDidEnterBackground(void);

	virtual void applicationWillEnterForeground(void);
};

#endif // _APP_DELEGATE_H_

