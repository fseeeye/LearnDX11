# Learn DirectX11 With Windows SDK

Based on :
- C++20
- HLSL Shader Model 5.0

## Get Started

1. Run `xmake project -k vsxmake`
2. Open the generated VS solution file in `vsxmake20xx` folder.
1. Choose **Release x64** and Build in Visual Studio.

## Core Dependencies
- [ocornut/imgui](https://github.com/ocornut/imgui)
- [nothings/stb](https://github.com/nothings/stb)
- [assimp/assimp](https://github.com/assimp/assimp)：model loading
- [DirectXTex/DDSTextureLoader](https://github.com/Microsoft/DirectXTex/tree/master/DDSTextureLoader)
- [DirectXTex/WICTextureLoader](https://github.com/Microsoft/DirectXTex/tree/master/WICTextureLoader)
- [DirectXTex/ScreenGrab](https://github.com/Microsoft/DirectXTex/tree/master/ScreenGrab)
- [DirectXTK/Mouse](https://github.com/Microsoft/DirectXTK/tree/master/Src)：Not compatible with imgui. (源码上有所修改)
- [DirectXTK/Keyboard](https://github.com/Microsoft/DirectXTK/tree/master/Src)：Not compatible with imgui. (源码上有所修改)

## File Structure

```plaintext
|----Model/
|----Texture/
|----xxx/
     |----xxx.exe
     |----imgui.ini
     |----assimp-vc14*-mt.dll  (从build/Assimp/bin/Release(或Debug)复制一份)
     |----Shaders/
```

## Ref

- DirectX11 With Windows SDK : https://directx11.tech/
