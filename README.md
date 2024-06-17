# Learn DirectX11 With Windows SDK

Based on :
- C++20
- HLSL Shader Model 5.0

## Get Started

1. (Optional)Install vcpkg : 
    1. `git clone https://github.com/microsoft/vcpkg.git`
    2. `cd vcpkg`
    3. `.\bootstrap-vcpkg.bat`
    4. add install path to system environment variable `VCPKG_ROOT` : `set VCPKG_ROOT="C:\path\to\vcpkg"; set PATH=%VCPKG_ROOT%;%PATH%`
2. Install xmake : `winget install xmake`
3. Develop with VS:
     1. Run `xmake project -k vsxmake -m "debug,release"`
     2. Open the generated VS solution file in `vsxmake20xx` folder.
     3. Choose **Release/Debug x64** and Build in Visual Studio.
4. Develop with VSCode:
     1. (Optional) Set Debug/Release mode: `xmake config -m [debug|release]` 
     2. Build: `xmake -v`
     3. Run: `xmake run xxx [-d]`

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
