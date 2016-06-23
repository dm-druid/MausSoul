#include "AppDelegate.h"
#include "BackgroundLayer.h"
#include "StartScene.h"
#include "SceneOne.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1080);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate(void) {
	return;
}

AppDelegate::~AppDelegate(void) {
	return;
}

void AppDelegate::initGLContextAttrs(void) {

	/*
	 * set OpenGL context attributions, now can only set six attributions:
	 * red, green, blue, alpha, depth, stencil
	 */
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

	GLView::setGLContextAttrs(glContextAttrs);
	return;
}

static int register_all_packages(void) {
	return 0; // flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching(void) {

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) \
	|| (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) \
	|| (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect(
			"Maus Soul",
			Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
		glview = GLViewImpl::create("Maus Soul");
#endif
		director->setOpenGLView(glview);
	}

	director->setDisplayStats(false);

	director->setAnimationInterval(1.0f / 60);

	glview->setDesignResolutionSize(
		designResolutionSize.width,
		designResolutionSize.height,
		ResolutionPolicy::NO_BORDER);
	Size frameSize = glview->getFrameSize();

	register_all_packages();

	// create a scene. it's an autorelease object
	auto startscene = StartScene::create();

	// run
	director->runWithScene(startscene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground(void) {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	return;
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground(void) {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	return;
}
