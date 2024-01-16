#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <vector>
#include <wrl.h> // ComPtr
#include "Define.h"

class Application;

using Microsoft::WRL::ComPtr;
using std::shared_ptr;
using std::vector;
using std::wstring;

class HRenderModule
{
#pragma region Init
public: 
	bool Initialize(Application* pAppContext);
private:
	bool InitDeviceAndContext();
	bool InitSwapChain();
	bool InitRasterizerState();
	bool InitRenderTargetView();
	bool InitDepthBuffer();
	bool InitDepthStencil();

	void SetViewport();

#pragma endregion

#pragma region Loop
public:
	void Run();

private:
	void Update();
	void Render();

#pragma endregion 

#pragma region Components


private:
	Application* m_AppContext = nullptr;

	ComPtr<ID3D11Device> m_device; //디바이스
	ComPtr<ID3D11DeviceContext> m_context; //컨텍스트 
	ComPtr<ID3D11RenderTargetView> m_renderTargetView; //렌더 타겟 뷰
	
	ComPtr<IDXGISwapChain> m_swapChain; //스왑체인

	D3D11_VIEWPORT m_screenViewport; //뷰포트
	
	//Rasterizer
	ComPtr<ID3D11RasterizerState> m_solidRasterizerState; //래스터라이저
	ComPtr<ID3D11RasterizerState> m_wireRasterizerState; //와이어 프레임

	//DepthStencil
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState; //뎁스 스텐실 스테이트

#pragma endregion


#pragma region Properties
private:
	UINT m_MSAAQuality;
	D3D_DRIVER_TYPE m_DriverType;
	UINT m_CreateDeviceFlags;

	D3D_FEATURE_LEVEL m_FeatureLevels[2] = {
		D3D_FEATURE_LEVEL_11_0, // 더 높은 버전이 먼저 오도록 설정
		D3D_FEATURE_LEVEL_9_3 };

	D3D_FEATURE_LEVEL m_UsingFeatureLevel;

#pragma endregion

};