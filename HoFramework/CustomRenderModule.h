#pragma once


#include "BaseRenderModule.h"

class HCube1RenderingObject;


class HCustomRenderModule : public HBaseRenderModule
{

public:
	HCustomRenderModule() : HBaseRenderModule()
	{

	}

	~HCustomRenderModule();

	virtual bool Initialize(Application* pAppContext) override;

private:
	void InitSampler();

protected:
	virtual void Update() override;
	virtual void Render() override;

protected:

protected:
	ComPtr<ID3D11SamplerState> m_SamplerState;

protected:
	HCube1RenderingObject* Cube1 = nullptr;




#pragma region View
protected:
	float ViewAngleInDeg = 70.f;

#pragma endregion


};