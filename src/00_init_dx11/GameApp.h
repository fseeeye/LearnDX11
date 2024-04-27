// 游戏应用程序扩展类，游戏逻辑在这里实现，继承自D3DApp类

#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
class GameApp : public D3DApp
{
public:
    GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~GameApp();

    bool Init();
    void OnResize();
    void UpdateScene(float dt);
    void DrawScene();
};


#endif