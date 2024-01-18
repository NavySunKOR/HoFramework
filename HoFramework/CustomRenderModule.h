#pragma once


#include "BaseRenderModule.h"


class HCustomRenderModule : public HBaseRenderModule
{

public:
	HCustomRenderModule() : HBaseRenderModule()
	{

	}


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
	vector<std::shared_ptr<class HBaseRenderingObject>> RenderingObjects;


#pragma region View
protected:
	float ViewAngleInDeg = 70.f;

#pragma endregion


};