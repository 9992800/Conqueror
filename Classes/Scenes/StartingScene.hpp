#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "PluginFacebook/PluginFacebook.h"

using namespace cocos2d::ui;
USING_NS_CC;

class Starting : public cocos2d::Layer, sdkbox::FacebookListener
{
public:
        static Scene* createScene();
        virtual bool init() override;
        CREATE_FUNC(Starting);

        void update(float delta)override;
        virtual void onExit()override;
        
protected:
        void menuStartGame(Ref* pSender);
        void menuHelp(Ref* pSender);
        void menuSoundCtrl(Ref* pSender);
        void menuShareGame(Ref* pSender);
        
        
        void onLogin(bool, const std::string&)override;
        void onSharedSuccess(const std::string&)override;
        void onSharedFailed(const std::string&)override;
        void onSharedCancel()override;
        void onAPI(const std::string&, const std::string&)override;
        void onPermission(bool, const std::string&)override;
        void onFetchFriends(bool, const std::string&)override;
        void onRequestInvitableFriends(const sdkbox::FBInvitableFriendsInfo&)override;
        void onInviteFriendsWithInviteIdsResult(bool, const std::string&)override;
        void onInviteFriendsResult(bool, const std::string&)override;
        void onGetUserInfo(const sdkbox::FBGraphUser&)override;

        
private:
        void afterCaptureScreen(bool, const std::string&);
        
private:
        int _count = 0;
        LoadingBar* _loadingBar;
        MenuItemImage* _startGame,
                        *_soundCtrl,
                        *_helpBtn,
                        *_shareIt;
        std::string _captureFilename;
};

#endif // __HELLOWORLD_SCENE_H__
