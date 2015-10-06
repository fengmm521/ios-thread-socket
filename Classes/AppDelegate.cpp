#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "ClientSocketManger.h"
#include "LocalFIleDataManger.h"
USING_NS_CC;
#define kWidthWindow 1536
#define kHeightWindow 2048

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    //cocos2d::ZipUtils::setPvrEncryptionKeyPart(2 , kImagePassWord2);
    log("app run 1 1...");
    auto director = Director::getInstance();
    //cocos2d::ZipUtils::setPvrEncryptionKeyPart(3 , kImagePassWord3);
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
        
    }
    
   // CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound/Click_Button.wav");
    Size si = Director::getInstance()->getWinSize();
    float xtmp = ((float)si.height/(float)si.width);
    if (xtmp < 1.6f) {
        //glview->setDesignResolutionSize(kWidthWindow, kHeightWindow, ResolutionPolicy::EXACT_FIT);
        glview->setDesignResolutionSize(kWidthWindow, kHeightWindow, ResolutionPolicy::SHOW_ALL);
    }else{
        glview->setDesignResolutionSize(kWidthWindow, kHeightWindow, ResolutionPolicy::EXACT_FIT);
    }
    

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    LocalFIleDataManger::sharedLocalFIleDataManger()->saveUserData();
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    if (ClientSocketManger::sharedClientSocketManger()->nettype != 11) {
        ClientSocketManger::sharedClientSocketManger()->connectSocket();
    }
    LocalFIleDataManger::sharedLocalFIleDataManger()->readUserData();
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
