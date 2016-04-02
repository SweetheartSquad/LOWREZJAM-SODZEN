#pragma once

#include <MY_Scene_Menu.h>
#include <MY_Scene_Main.h>
#include <shader/ComponentShaderText.h>
#include <sweet/UI.h>

MY_Scene_Menu::MY_Scene_Menu(Game * _game) :
	MY_Scene_Base(_game)
{
	TextLabel * t = new TextLabel(uiLayer->world, font, textShader);
	t->setText("press enter to start");
	uiLayer->addChild(t);
}

void MY_Scene_Menu::update(Step * _step){
	if(keyboard->keyJustDown(GLFW_KEY_ENTER)){
		game->scenes["main"] = new MY_Scene_Main(game);
		game->switchScene("main", false);
	}

	MY_Scene_Base::update(_step);
}