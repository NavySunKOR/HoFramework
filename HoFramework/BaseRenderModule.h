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

class HBaseRenderModule
{
#pragma region Init
public: 
	virtual bool Initialize(Application* pAppContext);
private:
	bool InitDeviceAndContext();
	bool InitSwapChain();
	bool InitRasterizerState();
	bool InitRenderTargetView();
	bool InitDepthBuffer();
	bool InitDepthStencil();
	bool InitSampler();


	void SetViewport();

#pragma endregion

#pragma region Loop
public:
	void Run()
	{
		Update();
		Render();
	};

protected:
	virtual void Update() {};
	virtual void Render() {
		
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
	m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	};

#pragma endregion 

#pragma region Components

public:

	inline Application* GetAppContext() {
		return m_AppContext;
	};

	inline ComPtr<ID3D11Device>& GetDevice() {
		return m_device;
	};

	inline ComPtr<ID3D11DeviceContext>& GetContext() {
		return m_context;
	};

	inline  ComPtr<IDXGISwapChain>& GetSwapChain() {
		return m_swapChain;
	};

	inline ComPtr<ID3D11RenderTargetView>& GetRenderTargetView() {
		return m_renderTargetView;
	};

	inline D3D11_VIEWPORT& GetScreenViewport() {
		return m_screenViewport;
	};

	inline bool IsPerspective() {
		return m_IsPersepective;
	};

	inline float GetFOVInDeg() {
		return m_FOVInDeg;
	};

	inline ComPtr<ID3D11SamplerState>& GetSampler()
	{
		return m_SamplerState;
	}

	inline vector<std::shared_ptr<class HBaseRenderingObject>>& GetRenderingObjects()
	{
		return RenderingObjects;
	}

	inline Vector3& GetGlobalCameraPosition()
	{
		return GlobalCameraPosition;
	}

	inline Vector3& GetGlobalCameraRotation()
	{
		return GlobalCameraRotation;
	}

	inline Vector3& GetGlobalCameraForward()
	{
		return GlobalCameraForward;
	}

protected:
	Application* m_AppContext = nullptr;

	ComPtr<ID3D11Device> m_device; //디바이스
	ComPtr<ID3D11DeviceContext> m_context; //컨텍스트 

	ComPtr<ID3D11RenderTargetView> m_renderTargetView; //렌더 타겟 뷰
	
	ComPtr<IDXGISwapChain> m_swapChain; //스왑체인

	D3D11_VIEWPORT m_screenViewport; //뷰포트
	
	//Rasterizer
	ComPtr<ID3D11RasterizerState> m_RasterizerState; //래스터라이저

	//DepthStencil
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState; //뎁스 스텐실 스테이트

	//Sampler
	ComPtr<ID3D11SamplerState> m_SamplerState;

	//RenderingObjects
	vector<std::shared_ptr<class HBaseRenderingObject>> RenderingObjects;

#pragma endregion

#pragma region Functions
public:
	void ResizeWindow();

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

	bool m_IsInitialized = false;

	
	//View 관련
	bool m_IsPersepective = true;
	float m_FOVInDeg = 70.f;

	Vector3 GlobalCameraPosition = Vector3(0.f, 0.f, 2.f);
	Vector3 GlobalCameraRotation = Vector3(0.f, 0.f, 0.f);
	Vector3 GlobalCameraForward = Vector3(0.f, 0.f, 1.f);

#pragma endregion

};