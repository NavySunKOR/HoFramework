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

protected:

	virtual void InitImageFilters() = 0;

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
	virtual void Update();
	virtual void Render();

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

	inline bool& GetIsWireframe()
	{
		return m_CurrentIsWireframe;
	}

protected:
	Application* m_AppContext = nullptr;

	ComPtr<ID3D11Device> m_device; //����̽�
	ComPtr<ID3D11DeviceContext> m_context; //���ؽ�Ʈ 

	ComPtr<ID3D11RenderTargetView> m_renderTargetView; //���� Ÿ�� ��
	ComPtr<ID3D11ShaderResourceView> m_renderTargetResourceView; //���� Ÿ���� ���ҽ�ȭ ��(Post Process Image Filter � ����)
	ComPtr<ID3D11Texture2D> m_tempTexture;// ����Ÿ���� Texture2DMS�� ������ �ӽ� �ؽ���
	
	ComPtr<IDXGISwapChain> m_swapChain; //����ü��

	D3D11_VIEWPORT m_screenViewport; //����Ʈ
	
	//Rasterizer
	ComPtr<ID3D11RasterizerState> m_RasterizerState; //�����Ͷ�����

	//DepthStencil
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState; //���� ���ٽ� ������Ʈ

	//Sampler
	ComPtr<ID3D11SamplerState> m_SamplerState;

	//RenderingObjects
	vector<std::shared_ptr<class HBaseRenderingObject>> RenderingObjects;
	vector< std::shared_ptr<class HImageFilter>> ImageFilters;

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
		D3D_FEATURE_LEVEL_11_0, // �� ���� ������ ���� ������ ����
		D3D_FEATURE_LEVEL_9_3 };

	D3D_FEATURE_LEVEL m_UsingFeatureLevel;

	bool m_IsInitialized = false;

	
	//View ����
	bool m_IsPersepective = true;
	float m_FOVInDeg = 70.f;

	Vector3 GlobalCameraPosition = Vector3(0.f, 0.f, 2.f);
	Vector3 GlobalCameraRotation = Vector3(0.f, 0.f, 0.f);
	Vector3 GlobalCameraForward = Vector3(0.f, 0.f, 1.f);


	//Wireframe����
	bool m_PrevIsWireframe = false;
	bool m_CurrentIsWireframe = false;

#pragma endregion

};