#pragma once

#include <MY_Scene_Main.h>
#include <RenderSurface.h>
#include <StandardFrameBuffer.h>
#include <RenderOptions.h>

#include <MeshFactory.h>

#include <CubeMap.h>

MY_Scene_Main::MY_Scene_Main(Game * _game) :
	MY_Scene_Base(_game),
	screenSurfaceShader(new Shader("assets/RenderSurface_1", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader, true)),
	screenFBO(new StandardFrameBuffer(true)),
	zoom(5),
	gameCamPolarCoords(0, zoom),
	orbitalSpeed(1),
	orbitalHeight(3),
	targetOrbitalHeight(orbitalHeight),
	mouseX(0),
	mouseY(0)
{
	// memory management
	screenSurface->incrementReferenceCount();
	screenSurfaceShader->incrementReferenceCount();
	screenFBO->incrementReferenceCount();

	screenSurface->setScaleMode(GL_NEAREST);
	

	MeshEntity * clouds = new MeshEntity(MY_ResourceManager::globalAssets->getMesh("CLOUDS")->meshes.at(0), baseShader);
	clouds->mesh->pushTexture2D(MY_ResourceManager::globalAssets->getTexture("CLOUDS")->texture);
	clouds->mesh->setScaleMode(GL_NEAREST);
	childTransform->addChild(clouds);

	MeshEntity * environment = new MeshEntity(MY_ResourceManager::globalAssets->getMesh("ENVIRONMENT")->meshes.at(0), baseShader);
	environment->mesh->pushTexture2D(MY_ResourceManager::globalAssets->getTexture("ENVIRONMENT")->texture);
	environment->mesh->setScaleMode(GL_NEAREST);
	childTransform->addChild(environment);

	MeshEntity * pot = new MeshEntity(MY_ResourceManager::globalAssets->getMesh("POT")->meshes.at(0), baseShader);
	pot->mesh->pushTexture2D(MY_ResourceManager::globalAssets->getTexture("POT")->texture);
	pot->mesh->setScaleMode(GL_NEAREST);
	childTransform->addChild(pot);

	grass = new MeshEntity(MY_ResourceManager::globalAssets->getMesh("GRASS")->meshes.at(0), baseShader);
	grass->mesh->pushTexture2D(MY_ResourceManager::globalAssets->getTexture("GRASS")->texture);
	grass->mesh->setScaleMode(GL_NEAREST);
	childTransform->addChild(grass);

	can = new MeshEntity(MY_ResourceManager::globalAssets->getMesh("CAN")->meshes.at(0), baseShader);
	can->mesh->pushTexture2D(MY_ResourceManager::globalAssets->getTexture("CAN")->texture);
	can->mesh->setScaleMode(GL_NEAREST);
	childTransform->addChild(can);

	sweet::setCursorMode(GLFW_CURSOR_NORMAL);

	gameCam = new PerspectiveCamera();
	childTransform->addChild(gameCam);
	cameras.push_back(gameCam);
	activeCamera = gameCam;
}

MY_Scene_Main::~MY_Scene_Main(){
	
	// memory management
	screenSurface->decrementAndDelete();
	screenSurfaceShader->decrementAndDelete();
	screenFBO->decrementAndDelete();
}


void MY_Scene_Main::update(Step * _step){
	// Screen shader update
	// Screen shaders are typically loaded from a file instead of built using components, so to update their uniforms
	// we need to use the OpenGL API calls
	screenSurfaceShader->bindShader(); // remember that we have to bind the shader before it can be updated
	GLint test = glGetUniformLocation(screenSurfaceShader->getProgramId(), "time");
	checkForGlError(0);
	if(test != -1){
		glUniform1f(test, _step->time);
		checkForGlError(0);
	}


	zoom -= mouse->getMouseWheelDelta();
	zoom = glm::clamp(zoom, 1.f, 10.f);
	gameCamPolarCoords.y += (zoom - gameCamPolarCoords.y) * 0.05f;
	gameCamPolarCoords.y = glm::clamp(gameCamPolarCoords.y, 1.f, 10.f);

	if(mouse->leftDown()){
		if(!mouse->leftJustPressed()){
			orbitalSpeed += ((mouse->mouseX() - mouseX) - orbitalSpeed) * 0.1f;
			targetOrbitalHeight -= ((mouse->mouseY() - mouseY)) * 0.025f;
		}
		mouseX = mouse->mouseX();
		mouseY = mouse->mouseY();
	}
	
	targetOrbitalHeight = glm::clamp(targetOrbitalHeight, 1.f, 10.f);
	orbitalSpeed = glm::clamp(orbitalSpeed, -15.f, 15.f);

	orbitalHeight += (targetOrbitalHeight - orbitalHeight) * 0.1f;

	gameCamPolarCoords.x += _step->deltaTimeCorrection * 0.005f * orbitalSpeed;

	gameCam->firstParent()->translate(glm::vec3(glm::sin(gameCamPolarCoords.x) * gameCamPolarCoords.y, orbitalHeight, glm::cos(gameCamPolarCoords.x) * gameCamPolarCoords.y), false);

	// Scene update
	MY_Scene_Base::update(_step);
	
	gameCam->lookAtSpot = glm::vec3(0,0,0);
	gameCam->forwardVectorRotated = gameCam->lookAtSpot - gameCam->getWorldPos();
	//gameCam->childTransform->lookAt(glm::vec3(0));
}

void MY_Scene_Main::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	glm::uvec2 sd = sweet::getWindowDimensions();
	int max = glm::max(sd.x, sd.y);
	int min = glm::min(sd.x, sd.y);
	bool horz = sd.x == max;
	int offset = (max - min)/2;

	// keep our screen framebuffer up-to-date with the current viewport
	screenFBO->resize(64, 64);
	_renderOptions->setViewPort(0,0,64,64);
	_renderOptions->setClearColour(1,0,1,0);

	// bind our screen framebuffer
	FrameBufferInterface::pushFbo(screenFBO);
	// render the scene
	MY_Scene_Base::render(_matrixStack, _renderOptions);
	// unbind our screen framebuffer, rebinding the previously bound framebuffer
	// since we didn't have one bound before, this will be the default framebuffer (i.e. the one visible to the player)
	FrameBufferInterface::popFbo();

	// render our screen framebuffer using the standard render surface
	_renderOptions->setViewPort(horz ? offset : 0, horz ? 0 : offset, min, min);
	screenSurface->render(screenFBO->getTextureId());

	// render the uiLayer after the screen surface in order to avoid hiding it through shader code
	uiLayer->render(_matrixStack, _renderOptions);
}

void MY_Scene_Main::load(){
	MY_Scene_Base::load();	

	screenSurface->load();
	screenFBO->load();
}

void MY_Scene_Main::unload(){
	screenFBO->unload();
	screenSurface->unload();

	MY_Scene_Base::unload();	
}