//
//  AchievementScene.cpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/12/27.
//
//
#include "APPConstants.hpp"
#include "AchievementScene.hpp"

Scene* Achievement::createScene(){
        auto scene = Scene::create();
        auto layer = Achievement::create();
        scene->addChild(layer);
        return scene;
}

bool Achievement::init() {
        auto visible_size = Director::getInstance()->getVisibleSize();
        auto scene_back = Sprite::create("shopping/shopping_back.png");
        scene_back->setPosition(visible_size / 2);
        auto scene_back_size = scene_back->getContentSize();
        this->addChild(scene_back);
        
        auto exit_btn = ui::Button::create("CloseNormal.png", "CloseSelected.png");
        exit_btn->setPosition(Vec2(visible_size.width - exit_btn->getContentSize().width * 0.6f,
                                   visible_size.height - exit_btn->getContentSize().height * 0.6f));
        
        exit_btn->addClickEventListener([=](Ref*){
                Director::getInstance()->popScene();
        });
        scene_back->addChild(exit_btn);
        
        auto title = Sprite::create("achievement/achieve_title.png");
        title->setPosition(Vec2(visible_size.width / 2, visible_size.height - title->getContentSize().height * 0.5f));
        scene_back->addChild(title);
        
        
        auto achive_item_back = this->createListItem();
        _listView = ui::ListView::create();
        // set list view ex direction
        _listView->setDirection(ui::ScrollView::Direction::VERTICAL);
        _listView->setBounceEnabled(true);  
        _listView->setContentSize(achive_item_back->getContentSize());
        _listView->setPosition((visible_size - title->getContentSize() - _listView->getContentSize() / 2.0f));
        _listView->setScrollBarPositionFromCorner(Vec2(7, 7));
        scene_back->addChild(_listView);
        
        _listView->setItemModel(achive_item_back);
        _listView->setGravity(ui::ListView::Gravity::CENTER_VERTICAL);
        _totalCount = 
        for (int i = 0; i < _totalCount; ++i) {
                if (i < 3) {
                        Widget* item = default_item->clone();
                        item->setTag(i);
                        Button* btn = (Button*)item->getChildByName("Title Button");
                        btn->setTitleText(_array.at(i));
                        _listView->pushBackCustomItem(item);
                }
        }
        
        
        _listView->setItemsMargin(8);
        this->scheduleUpdate();
        _listView->forceDoLayout();
        return true;
}


ui::Layout* Achievement::createListItem(){
        
        auto default_back =  Sprite::create("achievement/achieve_item_back.png");
        default_back->setName("item_back");

        ui::Layout* default_item = ui::Layout::create();
        default_item->setTouchEnabled(true);
        default_item->setContentSize(default_back->getContentSize());
        default_back->setPosition(Vec2(default_item->getContentSize() / 2.0f));
        default_item->addChild(default_back);
        
        
        
        
        return default_item;
}
