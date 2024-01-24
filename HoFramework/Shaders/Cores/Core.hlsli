//48byte
struct Material
{
	//16
	//���� - 16byte�� ��� �϶� 
    float3 diffuse;
    float shiness;
	
	//�̷��� �� ���� 16����Ʈ�� ���� �־�� �Ѵ� Vector3�� ���޾� ���� ���� Vector3 ������ x ��Ʈ�� ���� ����� �ȴ�.

	//16
    float3 ambient;
    float Materialdummy1;

	//16
    float3 specular;
    float Materialdummy2;

	//4 <-�������� �����Ͱ� ���� �ʾƼ� ������ �����.
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