//***************************************************************************************
// Frank Luna (C) 2011 All Rights Reserved.
// Direct3D Application Framework
//***************************************************************************************

#pragma once

#include <string>
#include <wrl/client.h>
#include <d3d11_1.h>

#include "GameTimer.h"

class D3DApp
{
public:
    D3DApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    virtual ~D3DApp();

    /**
     * 获取应用实例的句柄
     * @return 
     */
    HINSTANCE AppInst() const;
    /**
     * 获取主窗口句柄
     * @return 
     */
    HWND MainWnd() const;
    /**
     * 获取屏幕宽高比
     * @return 
     */
    float AspectRatio() const;

    /**
     * 运行程序，进行游戏主循环
     * @return 
     */
    int Run();

    /* 框架方法。客户派生类需要重载这些方法以实现特定的应用需求 */
    /**
     * 该父类方法需要初始化窗口和Direct3D部分
     * @return 
     */
    virtual bool Init();
    /**
     * 该父类方法需要在窗口大小变动的时候调用
     */
    virtual void OnResize();
    /**
     * 子类需要实现该方法，完成每一帧的更新
     * @param dt 
     */
    virtual void UpdateScene(float dt) = 0;
    /**
     * 子类需要实现该方法，完成每一帧的绘制
     */
    virtual void DrawScene() = 0;

    /**
     * 窗口的消息回调函数
     * @param hwnd 
     * @param msg 
     * @param wParam 
     * @param lParam 
     * @return 
     */
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
    /**
     * 窗口初始化
     * @return 
     */
    bool InitMainWindow();
    /**
     *  Direct3D初始化
     * @return 
     */
    bool InitDirect3D();

    /**
     * 计算每秒帧数并在窗口中显示
     */
    void CalculateFrameStats() const;

protected:
    HINSTANCE m_hAppInst; // 应用实例句柄
    HWND m_hMainWnd; // 主窗口句柄
    bool m_AppPaused; // 应用是否暂停
    bool m_Minimized; // 应用是否最小化
    bool m_Maximized; // 应用是否最大化
    bool m_Resizing; // 窗口大小是否变化
    bool m_Enable4xMsaa; // 是否开启4倍多重采样
    UINT m_4xMsaaQuality; // MSAA支持的质量等级

	// 自定义的窗口配置
	std::wstring m_MainWndCaption; // 主窗口标题
	int m_ClientWidth; // 视口宽度
	int m_ClientHeight; // 视口高度

    // 使用模板别名简化类型名 ComPtr
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
    // Direct3D 11
    ComPtr<ID3D11Device> m_pd3dDevice; // D3D11 Device : 用于创建各种所需资源，如: Resource、View、Shader等。
    ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext; // D3D11 Device Context : 用于设置渲染状态、绘制几何体等。
    ComPtr<IDXGISwapChain> m_pSwapChain;
    // Direct3D 11.1
    ComPtr<ID3D11Device1> m_pd3dDevice1; // D3D11.1 Device
    ComPtr<ID3D11DeviceContext1> m_pd3dImmediateContext1; // D3D11.1 Device Context
    ComPtr<IDXGISwapChain1> m_pSwapChain1;
    // 通用资源
    ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer; // 深度模板缓冲区
    ComPtr<ID3D11RenderTargetView> m_pRenderTargetView; // 渲染目标视图
    ComPtr<ID3D11DepthStencilView> m_pDepthStencilView; // 深度模板视图
    D3D11_VIEWPORT m_ScreenViewport; // 视口

	GameTimer m_Timer; // 计时器
};
