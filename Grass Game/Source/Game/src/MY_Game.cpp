#pragma once

#include <MY_Game.h>

#include <MY_ResourceManager.h>

#include <MY_Scene_Menu.h>
#include <MY_Scene_Box2D.h>
#include <MY_Scene_Bullet3D.h>
#include <MY_Scene_SurfaceShaders.h>
#include <MY_Scene_ScreenShaders.h>
#include <MY_Scene_VR.h>


MY_Game::MY_Game() :
	Game("menu", new MY_Scene_Menu(this), true) // initialize our game with a menu scene
{
	sweet::setCursorMode(GLFW_CURSOR_NORMAL);
}

MY_Game::~MY_Game(){}

void MY_Game::addSplashes(){
	// add default splashes
	//Game::addSplashes();

	// add custom splashes
	//addSplash(new Scene_Splash(this, MY_ResourceManager::globalAssets->getTexture("DEFAULT")->texture, MY_ResourceManager::globalAssets->getAudio("DEFAULT")->sound));
}

void MY_Game::update(Step * _step){
	glm::uvec2 sd = sweet::getWindowDimensions();
	if(sd.x != sd.y){
		int s = glm::min(sd.x, sd.y);
		glfwSetWindowSize(sweet::currentContext, s, s);
	}
	Game::update(_step);
}