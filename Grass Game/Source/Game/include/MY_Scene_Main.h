#pragma once

#include <MY_Scene_Base.h>

class RenderSurface;
class StandardFrameBuffer;
class ShaderComponentUvOffset;
class ShaderComponentHsv;

class MY_Scene_Main : public MY_Scene_Base{
public:
	Shader * screenSurfaceShader;
	RenderSurface * screenSurface;
	StandardFrameBuffer * screenFBO;

	Camera * gameCam;

	// target zoom level
	float zoom;
	glm::vec2 gameCamPolarCoords;
	float orbitalSpeed;
	float orbitalHeight, targetOrbitalHeight;
	float mouseX;
	float mouseY;

	ComponentShaderBase * grassShader;
	ShaderComponentUvOffset * grassShaderOffset;
	ShaderComponentHsv * grassShaderHsv;
	Timeout * grassDecayTimer;
	bool decaying;

	virtual void update(Step * _step) override;
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;

	virtual void unload() override;
	virtual void load() override;

	MY_Scene_Main(Game * _game);
	~MY_Scene_Main();
	
	MeshEntity * grass;
	MeshEntity * can;
};