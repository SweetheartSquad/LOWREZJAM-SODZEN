#pragma once

#include <MY_Scene_Menu.h>
#include <MY_Scene_Main.h>
#include <shader/ComponentShaderText.h>
#include <sweet/UI.h>

MY_Scene_Menu::MY_Scene_Menu(Game * _game) :
	MY_Scene_Base(_game)
{
	{
	VerticalLinearLayout * vl = new VerticalLinearLayout(uiLayer->world);
	uiLayer->addChild(vl);
	vl->setRationalHeight(1.f, uiLayer);
	vl->setRationalWidth(1.f, uiLayer);
	vl->horizontalAlignment = kCENTER;
	vl->verticalAlignment = kMIDDLE;
	NodeUI * menu = new NodeUI(uiLayer->world);
	vl->addChild(menu);
	menu->setRationalHeight(1.f, vl);
	menu->setSquareWidth(1.f);
	menu->background->mesh->pushTexture2D(MY_ResourceManager::globalAssets->getTexture("CLOUDS")->texture);
	menu->background->mesh->setScaleMode(GL_NEAREST);
	clouds = menu->background->mesh;
	}
	{
	VerticalLinearLayout * vl = new VerticalLinearLayout(uiLayer->world);
	uiLayer->addChild(vl);
	vl->setRationalHeight(1.f, uiLayer);
	vl->setRationalWidth(1.f, uiLayer);
	vl->horizontalAlignment = kCENTER;
	vl->verticalAlignment = kMIDDLE;
	NodeUI * menu = new NodeUI(uiLayer->world);
	vl->addChild(menu);
	menu->setRationalHeight(1.f, vl);
	menu->setSquareWidth(1.f);
	menu->background->mesh->pushTexture2D(MY_ResourceManager::globalAssets->getTexture("MENU")->texture);
	menu->background->mesh->setScaleMode(GL_NEAREST);
	}
}

void MY_Scene_Menu::update(Step * _step){
	if(mouse->leftJustReleased()){
		if(game->scenes.count("main") == 0){
			game->scenes["main"] = new MY_Scene_Main(game);
		}
		game->switchScene("main", false);
	}

	if(_step->cycles % 30 == 0){
		for(auto & v : clouds->vertices){
			v.u += 1/64.f;
		}
		clouds->dirty = true;
	}

	MY_Scene_Base::update(_step);
}