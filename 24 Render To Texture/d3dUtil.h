#ifndef D3DUTIL_H
#define D3DUTIL_H

#include <d3d11_1.h>			// �Ѱ���Windows.h
#include <DirectXCollision.h>	// �Ѱ���DirectXMath.h
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <wincodec.h>
#include <vector>
#include <string>
#include <filesystem>
#include "ScreenGrab.h"
#include "DDSTextureLoader.h"	
#include "WICTextureLoader.h"


// ------------------------------
// DXTraceW����
// ------------------------------
// �ڵ�����������������ʽ��������Ϣ����ѡ�Ĵ��󴰿ڵ���(�Ѻ���)
// [In]strFile			��ǰ�ļ�����ͨ�����ݺ�__FILEW__
// [In]hlslFileName     ��ǰ�кţ�ͨ�����ݺ�__LINE__
// [In]hr				����ִ�г�������ʱ���ص�HRESULTֵ
// [In]strMsg			���ڰ������Զ�λ���ַ�����ͨ������L#x(����ΪNULL)
// [In]bPopMsgBox       ���ΪTRUE���򵯳�һ����Ϣ������֪������Ϣ
// ����ֵ: �β�hr
HRESULT WINAPI DXTraceW(_In_z_ const WCHAR* strFile, _In_ DWORD dwLine, _In_ HRESULT hr, _In_opt_ const WCHAR* strMsg, _In_ bool bPopMsgBox);


// ------------------------------
// HR��
// ------------------------------
// Debugģʽ�µĴ���������׷��
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
	{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		{														\
			DXTraceW(__FILEW__, (DWORD)__LINE__, hr, L#x, true);\
		}														\
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif 
#endif

//
// ��ɫ��������غ���
//

// ------------------------------
// CreateShaderFromFile����
// ------------------------------
// [In]objFileNameInOut ����õ���ɫ���������ļ�(.cso)������ָ��������Ѱ�Ҹ��ļ�����ȡ
// [In]hlslFileName     ��ɫ�����룬��δ�ҵ���ɫ���������ļ��������ɫ������
// [In]entryPoint       ��ڵ�(ָ����ʼ�ĺ���)
// [In]shaderModel      ��ɫ��ģ�ͣ���ʽΪ"*s_5_0"��*����Ϊc,d,g,h,p,v֮һ
// [Out]ppBlobOut       �����ɫ����������Ϣ
HRESULT CreateShaderFromFile(const WCHAR * objFileNameInOut, const WCHAR * hlslFileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob ** ppBlobOut);


//
// ����������غ���
//

// ���ݸ�����DDS�����ļ����ϣ�����2D��������
// Ҫ�����������Ŀ��Ⱥ͸߶ȶ�һ��
// ��maxMipMapSizeΪ0��ʹ��Ĭ��mipmap�ȼ�
// ����mipmap�ȼ������ᳬ��maxMipMapSize
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateDDSTexture2DArrayFromFile(
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	const std::vector<std::wstring>& filenames,
	UINT maxMipMapSize = 0);



//
// ������������غ���
//

// ���ݸ�����һ�Ű������������������������������������
// Ҫ���������߱�Ϊ4:3���Ұ�������ʽ����:
// .  +Y .  .
// -X +Z +X -Z 
// .  -Y .  .
// �ú���Ĭ�ϲ�����mipmap(���ȼ���Ϊ1)������Ҫ������generateMipsΪtrue
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateWICTextureCubeFromFile(
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	std::wstring cubemapFileName,
	bool generateMips = false);

// ���ݰ�D3D11_TEXTURECUBE_FACE����˳�������������������������������
// Ҫ��������ͬ����С��������
// �ú���Ĭ�ϲ�����mipmap(���ȼ���Ϊ1)������Ҫ������generateMipsΪtrue
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateWICTextureCubeFromFile(
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext,
	std::vector<std::wstring> cubemapFileNames,
	bool generateMips = false);

#endif