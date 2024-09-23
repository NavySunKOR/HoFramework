#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <vector>
#include <wrl.h> // ComPtr
#include "Define.h"
#include "Camera.h"
#include "LightDefine.h"

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

protected:

	virtual void InitImageFilters() = 0;

private:
	bool InitDeviceAndContext();
	bool InitSwapChain();
	bool InitRenderTargetView();
	bool InitDepthBuffer();


#pragma endregion

#pragma region Loop
public:
	void Run()
	{
		Update();
		Render();
	};

protected:
	virtual void Update();
	virtual void Render();

	void SetPSO(const class HGraphicsPSO& InPSO);
	void SetViewport(D3D11_VIEWPORT* InViewport);


	void UpdateLightConstantData();

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


	inline vector<std::shared_ptr<class HBaseRenderingObject>>& GetRenderingObjects()
	{
		return RenderingObjects;
	}

	inline bool& GetIsWireframe()
	{
		return m_CurrentIsWireframe;
	}

	inline std::shared_ptr<class HSkyBoxRenderingObject> GetSkybox()
	{
		return SkyBoxObject;
	}

	inline HCamera& GetMainView() 
	{
		return m_MainView;
	}

	inline float* GetGamma()
	{
		return &Gamma;
	}

	inline float* GetExposure()
	{
		return &Exposure;
	}

	inline ComPtr<ID3D11Buffer> GetLightPSConstantBuffer()
	{
		return m_LightPSConstantBuffer;
	}


	Light m_Lights[3];

protected:
	Application* m_AppContext = nullptr;

	ComPtr<ID3D11Device> m_device; //디바이스
	ComPtr<ID3D11DeviceContext> m_context; //컨텍스트 

	ComPtr<ID3D11RenderTargetView> m_renderTargetView; //렌더 타겟 뷰
	ComPtr<ID3D11ShaderResourceView> m_renderTargetResourceView; //렌더 타겟을 리소스화 함(Post Process Image Filter 등에 사용됨)
	ComPtr<ID3D11Texture2D> m_tempTexture;// 렌더타겟의 Texture2DMS를 복사할 임시 텍스춰
	
	ComPtr<IDXGISwapChain> m_swapChain; //스왑체인

	//DepthStencil
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	//RenderingObjects
	std::shared_ptr<class HSkyBoxRenderingObject> SkyBoxObject;
	vector<std::shared_ptr<class HBaseRenderingObject>> RenderingObjects;
	vector< std::shared_ptr<class HImageFilter>> ImageFilters;

	//Light
	ComPtr<ID3D11Buffer> m_LightPSConstantBuffer;
	LightingPSConstantBuffer m_LightPSConstant;


	//Camera
	HCamera m_MainView;
	std::vector<HCamera> m_Views;

	float Gamma = 0.5f;
	float Exposure = 1;


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

	//Wireframe관련
	bool m_PrevIsWireframe = false;
	bool m_CurrentIsWireframe = false;

#pragma endregion

};