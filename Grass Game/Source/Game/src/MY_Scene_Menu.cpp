#pragma once

#include <MY_Scene_Menu.h>
#include <MY_Scene_Main.h>
#include <shader/ComponentShaderText.h>
#include <sweet/UI.h>

MY_Scene_Menu::MY_Scene_Menu(Game * _game) :
	MY_Scene_Base(_game)
{
	NodeUI * menu1 = new NodeUI(uiLayer->world);
	uiLayer->addChild(menu1);
	menu1->setRationalHeight(1.f, uiLayer);
	menu1->setSquareWidth(1.f);
	menu1->background->mesh->pushTexture2D(MY_ResourceManager::globalAssets->getTexture("MENU")->texture);
	menu1->background->mesh->setScaleMode(GL_NEAREST);
}

void MY_Scene_Menu::update(Step * _step){
	uiLayer->resize(0,0,64,64);
	if(keyboard->keyJustDown(GLFW_KEY_ENTER)){
		game->scenes["main"] = new MY_Scene_Main(game);
		game->switchScene("main", false);
	}

	MY_Scene_Base::update(_step);
}