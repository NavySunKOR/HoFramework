#pragma once


#include "BaseRenderModule.h"


class HCustomRenderModule : HBaseRenderModule
{

public:
	virtual bool Initialize(Application* pAppContext) override;

private:
	void InitSampler();


protected:
	virtual void Update() override;
	virtual void Render() override;


protected:
	ComPtr<ID3D11SamplerState> m_SamplerState;

protected:

	//여기서부터
	Mesh m_drawingMesh;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11Buffer> m_transformConstBuffer;
	TransformConstantBuffer m_transformConstData;

	float RoationYValue;

	//여기서 까지 하나의 오브젝트



#pragma region View
protected:
	float ViewAngleInDeg = 70.f;

#pragma endregion


};