//48byte
struct Material
{
	//16
	//주의 - 16byte씩 페어 일때 
    float3 diffuse;
    float shiness;
	
	//이렇게 한 쌍이 16바이트로 묶여 있어야 한다 Vector3를 연달아 쓰면 다음 Vector3 변수의 x 파트에 덮어 씌우게 된다.

	//16
    float3 ambient;
    float Materialdummy1;

	//16
    float3 specular;
    float Materialdummy2;

	//4 <-마지막은 데이터가 들어가지 않아서 구색만 맞춘것.
    float Materialdummy3;
};


//32byte
struct Light
{
	//16
    float3 LightPos; //12
    float LightIntensity; //4 

	//16
    float3 LightDir; //12
    float LightDummy; //4 
};

float3 BlinnPhongModel(float3 pLightDir, float3 pToViewDirection, float3 pNormalVector, float pLightIntensity, Material pMat)
{
    float3 halfWay = normalize(pToViewDirection + pLightDir);
    float hdotN = dot(halfWay, pNormalVector);
    float specular = pMat.specular * pow(max(hdotN, 0.f), pMat.shiness);
    
    return pMat.ambient + (pMat.diffuse + specular) * pLightIntensity;
}

float3 ComputeDirectionalLight(Light pLight, float3 pToViewDirection, float3 pNormalVector, Material pMat)
{
    float3 LightVec = -pLight.LightDir;
    float AppliedIntensity = pLight.LightIntensity * max(dot(LightVec, pNormalVector), 0.f);
    return BlinnPhongModel(LightVec, pToViewDirection, pNormalVector, AppliedIntensity, pMat);
    
}

struct VSInput
{
    float3 Position : POSITION;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};

struct PSInput
{
    float4 ScreenPosition : SV_POSITION;
    float3 WorldPosition : POSITION;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};


cbuffer TransformConstBuffer : register(b0)
{
    matrix ModelTransform;
    matrix InverseTransform;
    matrix ViewTransform;
    matrix ProjectionTransform;
}