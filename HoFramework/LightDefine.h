#pragma once
#include "Define.h"
#include "Camera.h"


struct LightPSConstantData
{
	Vector3 LightPos = Vector3(0.f, 0.f, 0.f);
	float LightIntensity = 0.f;

	Vector3 LightDir = Vector3(0.f, 0.f, 1.f);
	float FalloffStart = 0.f;

	float FalloffEnd = 5.f;
	float SpotFactor = 0.f;
	float Dummy1;
	float Dummy2;

	Vector3 LightColor = Vector3(1.f, 1.f, 1.f);
	float Dummy3;

};


//48bytes
struct Light
{
	LightPSConstantData ConstantData;
	HCamera LightView;
};


struct LightingPSConstantBuffer
{
	LightPSConstantData Lights[3];
};


static_assert((sizeof(LightingPSConstantBuffer) % 16) == 0,
	"Constant Data size must be 16-byte aligned");