//
//  AchievementScene.cpp
//  Conqueror
//
//  Created by Wansheng Li on 2016/12/27.
//
//
#include "AchievementScene.hpp"
enum{
        k_item_title_backgrd = 1,
        k_item_title_text,
        k_item_desc_text,
        k_item_get_btn,

};

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
        
        
        auto default_item = this->createListItem();
        _listView = ui::ListView::create();
        // set list view ex direction
        _listView->setDirection(ui::ScrollView::Direction::VERTICAL);
        _listView->setBounceEnabled(true);  
        _listView->setContentSize(Size(default_item->getContentSize().width,
                                       visible_size.height * 0.8f));
        _listView->setPosition(Vec2(visible_size.width * 0.02f, visible_size.height * 0.05f));
        _listView->setScrollBarPositionFromCorner(Vec2(7, 7));
        scene_back->addChild(_listView);
        
        _listView->setItemModel(default_item);
        _listView->setGravity(ui::ListView::Gravity::CENTER_VERTICAL);
        _achievementData = GolbalConfig::getInstance()->getAchievementData();
        _totalCount = (int)_achievementData.size();
        _spawnCount = 3;
        for (int i = 0; i < _totalCount; ++i) {
                if (i < _spawnCount) {
                        ui::Widget* item = default_item->clone();
                        this->initItemDetails(item, i);
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
        
        ui::Layout* default_item = ui::Layout::create();
        default_item->setBackGroundImage("achievement/achieve_item_back.png");
        auto default_item_size = default_back->getContentSize();
        default_item->setContentSize(default_item_size);
        default_item_size.width += 50;
        
        auto achieve_title = ui::ImageView::create("achievement/achieve_title_back.png");
        auto title_size = achieve_title->getContentSize();
        achieve_title->setPosition(Vec2(title_size.width * 0.45f,default_item_size.height - title_size.height * 0.5f));
        default_item->addChild(achieve_title, 1, k_item_title_backgrd);
        
        
        auto item_tile_txt = ui::Text::create("Achievement Item", "fonts/arial.ttf", 28);
        item_tile_txt->setPosition(Vec2(title_size.width *0.5f, title_size.height * 0.8f));
        achieve_title->addChild(item_tile_txt, 1, k_item_title_text);
        
        auto achieve_cup = ui::ImageView::create("achievement/achive_flag.png");
        auto achieve_cup_size = achieve_cup->getContentSize();
        achieve_cup->setPosition(achieve_cup_size * 0.6f);
        default_item->addChild(achieve_cup, 2);
        
        auto item_desc_txt = ui::Text::create("This is the description of the achievement.", "fonts/arial.ttf", 32);
        item_desc_txt->setPosition(Vec2(achieve_cup_size.width * 1.2f,
                                        default_item_size.height * 0.8f));
        item_desc_txt->setColor(Color3B::BLACK);
        default_item->addChild(item_desc_txt, 3, k_item_desc_text);
        
        
        auto butt_on = ui::Button::create("DIALOG2_OK.png","DIALOG2_OK_SEL.png");
        auto butt_on_size = butt_on->getContentSize();
        butt_on->setPosition(Vec2(default_item_size.width - butt_on_size.width,
                                  default_item_size.height * 0.5f));
        butt_on->setTouchEnabled(true);
        butt_on->setTitleText("Get it");
        butt_on->setTitleFontName("");
        butt_on->setTitleFontSize(42);
        default_item->addChild(butt_on, 4, k_item_get_btn);
        
        return default_item;
}

void Achievement::initItemDetails(ui::Widget* achieve_item, int idx){
        achieve_item->setTag(idx);
}
