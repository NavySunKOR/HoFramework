#pragma once
#include "BaseRenderingObject.h"

class HCube3RenderingObject : public HBaseRenderingObject
{
public:
	HCube3RenderingObject(HBaseRenderModule* pRenderModule) : HBaseRenderingObject(pRenderModule)
	{

	}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	float RotationYValue = 0.f;
	float RotationXValue = 0.f;
	Vector3 MovementDir = Vector3(1.f,0.f,0.f);
	float MovementScalar = 0.f;

	float MaxMovement = 0.1f;

	float ViewAngleInDeg = 70.f;

	bool IsRightDriection = true;


};