//***************************************************************************************
// Game Application Class (Inherited from D3DApp class), impl game logic.
//***************************************************************************************

#pragma once

#include "d3dApp.h"

class GameApp final : public D3DApp
{
public:
	GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
	~GameApp() override = default;

	bool Init() override;
	void OnResize() override;
	void UpdateScene(float dt) override;
	void DrawScene() override;
};
